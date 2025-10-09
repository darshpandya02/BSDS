#ifndef SERVER_STUB_H
#define SERVER_STUB_H

#include "Common.h"
#include "SocketComm.h"
#include <vector>
#include <arpa/inet.h>

class ServerStub {
private:
    SocketComm* comm;
    bool owns_comm; // whether this stub owns the SocketComm object
    
    // Unmarshal Order from byte stream
    Order UnmarshalOrder(const std::vector<char>& buffer) {
        Order order;
        size_t offset = 0;
        
        // Message type (skip)
        int msg_type;
        memcpy(&msg_type, buffer.data() + offset, sizeof(int));
        msg_type = ntohl(msg_type);
        offset += sizeof(int);
        
        if (msg_type != MSG_ORDER) {
            throw std::runtime_error("Invalid message type received");
        }
        
        // Customer ID
        int cid;
        memcpy(&cid, buffer.data() + offset, sizeof(int));
        order.customer_id = ntohl(cid);
        offset += sizeof(int);
        
        // Order number
        int onum;
        memcpy(&onum, buffer.data() + offset, sizeof(int));
        order.order_number = ntohl(onum);
        offset += sizeof(int);
        
        // Robot type
        int rtype;
        memcpy(&rtype, buffer.data() + offset, sizeof(int));
        order.robot_type = ntohl(rtype);
        
        return order;
    }
    
    // Marshal RobotInfo into byte stream
    std::vector<char> MarshalRobotInfo(const RobotInfo& robot) {
        std::vector<char> buffer;
        
        // Message type
        int msg_type = htonl(MSG_ROBOT);
        buffer.insert(buffer.end(), (char*)&msg_type, (char*)&msg_type + sizeof(int));
        
        // Customer ID
        int cid = htonl(robot.customer_id);
        buffer.insert(buffer.end(), (char*)&cid, (char*)&cid + sizeof(int));
        
        // Order number
        int onum = htonl(robot.order_number);
        buffer.insert(buffer.end(), (char*)&onum, (char*)&onum + sizeof(int));
        
        // Robot type
        int rtype = htonl(robot.robot_type);
        buffer.insert(buffer.end(), (char*)&rtype, (char*)&rtype + sizeof(int));
        
        // Engineer ID
        int engid = htonl(robot.engineer_id);
        buffer.insert(buffer.end(), (char*)&engid, (char*)&engid + sizeof(int));
        
        // Expert ID
        int expid = htonl(robot.expert_id);
        buffer.insert(buffer.end(), (char*)&expid, (char*)&expid + sizeof(int));
        
        return buffer;
    }
    
public:
    ServerStub() : comm(nullptr), owns_comm(false) {}
    
    ~ServerStub() {
        if (comm && owns_comm) {
            delete comm;
        }
    }
    
    // Initialize with a connected socket (takes ownership)
    void Init(SocketComm* connected_socket) {
        if (comm && owns_comm) {
            delete comm;
        }
        comm = connected_socket;
        owns_comm = true;
        
        if (!comm || !comm->IsConnected()) {
            throw std::runtime_error("Invalid or unconnected socket");
        }
    }
    
    // Alternative: Initialize with socket file descriptor
    void Init(int socket_fd) {
        if (comm && owns_comm) {
            delete comm;
        }
        comm = new SocketComm(socket_fd);
        owns_comm = true;
    }
    
    // Receive order from client
    Order ReceiveOrder() {
        if (!comm || !comm->IsConnected()) {
            throw std::runtime_error("Server stub not initialized");
        }
        
        // Receive order (4 integers: msg_type + 3 order fields)
        std::vector<char> order_data(4 * sizeof(int));
        comm->Receive(order_data.data(), order_data.size());
        
        // Unmarshal order
        return UnmarshalOrder(order_data);
    }
    
    // Send robot information to client
    void ShipRobot(const RobotInfo& robot) {
        if (!comm || !comm->IsConnected()) {
            throw std::runtime_error("Server stub not initialized");
        }
        
        // Marshal robot info
        std::vector<char> robot_data = MarshalRobotInfo(robot);
        
        // Send robot info
        comm->Send(robot_data.data(), robot_data.size());
    }
};

#endif // SERVER_STUB_H