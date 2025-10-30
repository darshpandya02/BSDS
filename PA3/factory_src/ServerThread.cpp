// #include <iostream>
// #include <memory>

// #include "ServerThread.h"
// #include "ServerStub.h"

// RobotInfo RobotFactory::CreateRegularRobot(RobotOrder order, int engineer_id) {
// 	RobotInfo robot;
// 	robot.CopyOrder(order);
// 	robot.SetEngineerId(engineer_id);
// 	robot.SetExpertId(-1);
// 	return robot;
// }

// RobotInfo RobotFactory::CreateSpecialRobot(RobotOrder order, int engineer_id) {
// 	RobotInfo robot;
// 	robot.CopyOrder(order);
// 	robot.SetEngineerId(engineer_id);

// 	std::promise<RobotInfo> prom;
// 	std::future<RobotInfo> fut = prom.get_future();

// 	std::unique_ptr<ExpertRequest> req = std::unique_ptr<ExpertRequest>(new ExpertRequest);
// 	req->robot = robot;
// 	req->prom = std::move(prom);

// 	erq_lock.lock();
// 	erq.push(std::move(req));
// 	erq_cv.notify_one();
// 	erq_lock.unlock();

// 	robot = fut.get();
// 	return robot;
// }

// void RobotFactory::EngineerThread(std::unique_ptr<ServerSocket> socket, int id) {
// 	int engineer_id = id;
// 	int robot_type;
// 	RobotOrder order;
// 	RobotInfo robot;

// 	ServerStub stub;

// 	stub.Init(std::move(socket));

// 	while (true) {
// 		order = stub.ReceiveOrder();
// 		if (!order.IsValid()) {
// 			break;	
// 		}
// 		robot_type = order.GetRobotType();
// 		switch (robot_type) {
// 			case 0:
// 				robot = CreateRegularRobot(order, engineer_id);
// 				break;
// 			case 1:
// 				robot = CreateSpecialRobot(order, engineer_id);
// 				break;
// 			default:
// 				std::cout << "Undefined robot type: "
// 					<< robot_type << std::endl;

// 		}
// 		stub.SendRobot(robot);
// 	}
// }

// void RobotFactory::ExpertThread(int id) {
// 	std::unique_lock<std::mutex> ul(erq_lock, std::defer_lock);
// 	while (true) {
// 		ul.lock();

// 		if (erq.empty()) {
// 			erq_cv.wait(ul, [this]{ return !erq.empty(); });
// 		}

// 		auto req = std::move(erq.front());
// 		erq.pop();

// 		ul.unlock();

// 		std::this_thread::sleep_for(std::chrono::microseconds(100));
// 		req->robot.SetExpertId(id);
// 		req->prom.set_value(req->robot);	
// 	}
// }



#include <iostream>
#include <memory>

#include "ServerThread.h"
#include "ServerStub.h"

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
        record.SetRecord(-1, -1);  // Not found
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

        // Create log entry
        MapOp op;
        op.opcode = 1;  // Update operation
        op.arg1 = req->request.GetCustomerId();
        op.arg2 = req->request.GetOrderNumber();

        // Append to log and apply to map
        record_lock.lock();
        smr_log.push_back(op);
        customer_record[op.arg1] = op.arg2;
        record_lock.unlock();

        // Notify engineer that update is complete
        req->prom.set_value(id);
    }
}