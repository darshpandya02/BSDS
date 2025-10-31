
// #include <iostream>
// #include <memory>

// #include "ServerThread.h"
// #include "ServerStub.h"

// RobotInfo RobotFactory::CreateRobotAndUpdateRecord(CustomerRequest request, int engineer_id) {
//     RobotInfo robot;
//     robot.CopyRequest(request);
//     robot.SetEngineerId(engineer_id);

//     // Send request to admin and wait for response
//     std::promise<int> prom;
//     std::future<int> fut = prom.get_future();

//     std::unique_ptr<AdminRequest> req = std::unique_ptr<AdminRequest>(new AdminRequest);
//     req->request = request;
//     req->engineer_id = engineer_id;
//     req->prom = std::move(prom);

//     arq_lock.lock();
//     arq.push(std::move(req));
//     arq_cv.notify_one();
//     arq_lock.unlock();

//     int admin_id = fut.get();
//     robot.SetAdminId(admin_id);
    
//     return robot;
// }

// CustomerRecord RobotFactory::ReadCustomerRecord(int customer_id) {
//     CustomerRecord record;
    
//     record_lock.lock();
//     auto it = customer_record.find(customer_id);
//     if (it != customer_record.end()) {
//         record.SetRecord(customer_id, it->second);
//     } else {
//         record.SetRecord(-1, -1);  // Not found
//     }
//     record_lock.unlock();
    
//     return record;
// }

// void RobotFactory::EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
//     int engineer_id = id;
//     CustomerRequest request;
//     RobotInfo robot;
//     CustomerRecord record;

//     ServerStub stub;
//     stub.Init(std::move(socket));

//     while (true) {
//         request = stub.ReceiveRequest();
//         if (!request.IsValid()) {
//             break;
//         }
        
//         int request_type = request.GetRequestType();
//         switch (request_type) {
//             case 1:  // Robot order
//                 robot = CreateRobotAndUpdateRecord(request, engineer_id);
//                 stub.ShipRobot(robot);
//                 break;
//             case 2:  // Customer record read
//                 record = ReadCustomerRecord(request.GetCustomerId());
//                 stub.ReturnRecord(record);
//                 break;
//             default:
//                 std::cout << "Undefined request type: " << request_type << std::endl;
//         }
//     }
// }

// void RobotFactory::AdminThread(int id) {
//     std::unique_lock<std::mutex> ul(arq_lock, std::defer_lock);
    
//     while (true) {
//         ul.lock();
//         if (arq.empty()) {
//             arq_cv.wait(ul, [this]{ return !arq.empty(); });
//         }

//         auto req = std::move(arq.front());
//         arq.pop();
//         ul.unlock();

//         // Create log entry
//         MapOp op;
//         op.opcode = 1;  // Update operation
//         op.arg1 = req->request.GetCustomerId();
//         op.arg2 = req->request.GetOrderNumber();

//         // Append to log and apply to map
//         record_lock.lock();
//         smr_log.push_back(op);
//         customer_record[op.arg1] = op.arg2;
//         record_lock.unlock();

//         // Notify engineer that update is complete
//         req->prom.set_value(id);
//     }
// }

#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"
#include "ClientStub.h"

RobotFactory::RobotFactory(int id, std::vector<PeerInfo> peer_list) {
    factory_id = id;
    primary_id = -1;
    peers = peer_list;
    last_index = -1;
    committed_index = -1;
    connections_established = false;
}

void RobotFactory::EstablishBackupConnections() {
    connection_lock.lock();
    if (connections_established) {
        connection_lock.unlock();
        return;
    }

    for (auto& peer : peers) {
        auto client_socket = std::unique_ptr<ClientSocket>(new ClientSocket());
        if (!client_socket->Init(peer.ip, peer.port)) {
            std::cout << "Failed to connect to peer " << peer.id << std::endl;
            connection_lock.unlock();
            return;
        }

        // Send identification message (1 = PFA)
        IdentificationMsg id_msg;
        id_msg.SetIdType(1);
        char buffer[32];
        id_msg.Marshal(buffer);
        if (!client_socket->Send(buffer, id_msg.Size(), 0)) {
            std::cout << "Failed to send identification to peer " << peer.id << std::endl;
            connection_lock.unlock();
            return;
        }

        backup_connections.push_back(std::move(client_socket));
    }

    connections_established = true;
    connection_lock.unlock();
}

bool RobotFactory::ReplicateToBackup(int backup_index, MapOp& op) {
    ReplicationRequest req;
    ReplicationResponse resp;
    char buffer[64];

    req.SetRequest(factory_id, committed_index, last_index, op.opcode, op.arg1, op.arg2);
    req.Marshal(buffer);

    if (!backup_connections[backup_index]->Send(buffer, req.Size(), 0)) {
        return false;
    }

    if (!backup_connections[backup_index]->Recv(buffer, resp.Size(), 0)) {
        return false;
    }

    resp.Unmarshal(buffer);
    return (resp.GetStatus() == 0);
}

RobotInfo RobotFactory::CreateRobotAndUpdateRecord(CustomerRequest request, int engineer_id) {
    RobotInfo robot;
    robot.CopyRequest(request);
    robot.SetEngineerId(engineer_id);

    // Send request to admin and wait for response
    std::promise<int> prom;
    std::future<int> fut = prom.get_future();

    std::unique_ptr<AdminRequest> req = std::unique_ptr<AdminRequest>(new AdminRequest);
    req->request = request;
    req->engineer_id = engineer_id;
    req->prom = std::move(prom);

    arq_lock.lock();
    arq.push(std::move(req));
    arq_cv.notify_one();
    arq_lock.unlock();

    int admin_id = fut.get();
    robot.SetAdminId(admin_id);
    
    return robot;
}

CustomerRecord RobotFactory::ReadCustomerRecord(int customer_id) {
    CustomerRecord record;
    
    record_lock.lock();
    auto it = customer_record.find(customer_id);
    if (it != customer_record.end()) {
        record.SetRecord(customer_id, it->second);
    } else {
        record.SetRecord(-1, -1);
    }
    record_lock.unlock();
    
    return record;
}

void RobotFactory::EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
    int engineer_id = id;
    CustomerRequest request;
    RobotInfo robot;
    CustomerRecord record;

    ServerStub stub;
    stub.Init(std::move(socket));

    // Receive identification message
    IdentificationMsg id_msg = stub.ReceiveIdentification();
    int id_type = id_msg.GetIdType();

    if (id_type == 1) {
        // This is a PFA connection - act as IFA
        IdleFactoryAdminThread(stub.GetSocket());
        return;
    }

    // This is a customer connection - act as Engineer
    while (true) {
        request = stub.ReceiveRequest();
        if (!request.IsValid()) {
            break;
        }
        
        int request_type = request.GetRequestType();
        switch (request_type) {
            case 1:  // Robot order
                robot = CreateRobotAndUpdateRecord(request, engineer_id);
                stub.ShipRobot(robot);
                break;
            case 2:  // Customer record read
                record = ReadCustomerRecord(request.GetCustomerId());
                stub.ReturnRecord(record);
                break;
            default:
                std::cout << "Undefined request type: " << request_type << std::endl;
        }
    }
}

void RobotFactory::AdminThread(int id) {
    std::unique_lock<std::mutex> ul(arq_lock, std::defer_lock);
    
    while (true) {
        ul.lock();
        if (arq.empty()) {
            arq_cv.wait(ul, [this]{ return !arq.empty(); });
        }

        auto req = std::move(arq.front());
        arq.pop();
        ul.unlock();

        // Set primary_id if not set and establish connections
        if (primary_id != factory_id) {
            primary_id = factory_id;
            EstablishBackupConnections();
        }

        // Create log entry
        MapOp op;
        op.opcode = 1;
        op.arg1 = req->request.GetCustomerId();
        op.arg2 = req->request.GetOrderNumber();

        // Append to log
        record_lock.lock();
        smr_log.push_back(op);
        last_index++;
        record_lock.unlock();

        // Replicate to all backups sequentially
        for (size_t i = 0; i < backup_connections.size(); i++) {
            if (!ReplicateToBackup(i, op)) {
                std::cout << "Replication to backup " << i << " failed" << std::endl;
            }
        }

        // Apply to local map and commit
        record_lock.lock();
        customer_record[op.arg1] = op.arg2;
        committed_index = last_index;
        record_lock.unlock();

        // Notify engineer
        req->prom.set_value(id);
    }
}

void RobotFactory::IdleFactoryAdminThread(std::unique_ptr<ServerSocket> socket) {
    ReplicationRequest req;
    ReplicationResponse resp;
    char buffer[64];

    while (true) {
        // Receive replication request
        if (!socket->Recv(buffer, req.Size(), 0)) {
            break;
        }
        req.Unmarshal(buffer);

        record_lock.lock();

        // Set primary_id if not set
        if (primary_id != req.GetFactoryId()) {
            primary_id = req.GetFactoryId();
        }

        // Write MapOp to log at req.last_index
        MapOp op;
        op.opcode = req.GetOpcode();
        op.arg1 = req.GetArg1();
        op.arg2 = req.GetArg2();

        // Ensure log is large enough
        while (smr_log.size() <= (size_t)req.GetLastIndex()) {
            MapOp empty_op;
            empty_op.opcode = 0;
            empty_op.arg1 = 0;
            empty_op.arg2 = 0;
            smr_log.push_back(empty_op);
        }

        smr_log[req.GetLastIndex()] = op;
        last_index = req.GetLastIndex();

        // Apply committed entries to customer_record
        while (committed_index < req.GetCommittedIndex()) {
            committed_index++;
            MapOp& committed_op = smr_log[committed_index];
            if (committed_op.opcode == 1) {
                customer_record[committed_op.arg1] = committed_op.arg2;
            }
        }

        record_lock.unlock();

        // Send response
        resp.SetStatus(0);
        resp.Marshal(buffer);
        if (!socket->Send(buffer, resp.Size(), 0)) {
            break;
        }
    }
}