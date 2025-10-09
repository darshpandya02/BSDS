#ifndef CLIENT_STUB_H
#define CLIENT_STUB_H

#include "Common.h"
#include "SocketComm.h"
#include <vector>
#include <arpa/inet.h>

class ClientStub {
private:
    SocketComm* comm;
    
    // Marshal Order into byte stream
    std::vector<char> MarshalOrder(const Order& order) {
        std::vector<char> buffer;
        
        // Message type
        int msg_type = htonl(MSG_ORDER);
        buffer.insert(buffer.end(), (char*)&msg_type, (char*)&msg_type + sizeof(int));
        
        // Customer ID
        int cid = htonl(order.customer_id);
        buffer.insert(buffer.end(), (char*)&cid, (char*)&cid + sizeof(int));
        
        // Order number
        int onum = htonl(order.order_number);
        buffer.insert(buffer.end(), (char*)&onum, (char*)&onum + sizeof(int));
        
        // Robot type
        int rtype = htonl(order.robot_type);
        buffer.insert(buffer.end(), (char*)&rtype, (char*)&rtype + sizeof(int));
        
        return buffer;
    }
    
    // Unmarshal RobotInfo from byte stream
    RobotInfo UnmarshalRobotInfo(const std::vector<char>& buffer) {
        RobotInfo robot;
        size_t offset = 0;
        
        // Message type (skip)
        int msg_type;
        memcpy(&msg_type, buffer.data() + offset, sizeof(int));
        msg_type = ntohl(msg_type);
        offset += sizeof(int);
        
        if (msg_type != MSG_ROBOT) {
            throw std::runtime_error("Invalid message type received");
        }
        
        // Customer ID
        int cid;
        memcpy(&cid, buffer.data() + offset, sizeof(int));
        robot.customer_id = ntohl(cid);
        offset += sizeof(int);
        
        // Order number
        int onum;
        memcpy(&onum, buffer.data() + offset, sizeof(int));
        robot.order_number = ntohl(onum);
        offset += sizeof(int);
        
        // Robot type
        int rtype;
        memcpy(&rtype, buffer.data() + offset, sizeof(int));
        robot.robot_type = ntohl(rtype);
        offset += sizeof(int);
        
        // Engineer ID
        int engid;
        memcpy(&engid, buffer.data() + offset, sizeof(int));
        robot.engineer_id = ntohl(engid);
        offset += sizeof(int);
        
        // Expert ID
        int expid;
        memcpy(&expid, buffer.data() + offset, sizeof(int));
        robot.expert_id = ntohl(expid);
        
        return robot;
    }
    
public:
    ClientStub() : comm(nullptr) {}
    
    ~ClientStub() {
        if (comm) {
            delete comm;
        }
    }
    
    // Initialize client stub and connect to server
    bool Init(const std::string& ip, int port) {
        try {
            comm = new SocketComm();
            return comm->Connect(ip, port);
        } catch (const std::exception& e) {
            if (comm) {
                delete comm;
                comm = nullptr;
            }
            throw;
        }
    }
    
    // Send order and receive robot information
    RobotInfo Order(const Order& order) {
        if (!comm || !comm->IsConnected()) {
            throw std::runtime_error("Client stub not initialized");
        }
        
        // Marshal order
        std::vector<char> order_data = MarshalOrder(order);
        
        // Send order
        comm->Send(order_data.data(), order_data.size());
        
        // Receive robot info (6 integers: msg_type + 5 robot fields)
        std::vector<char> robot_data(6 * sizeof(int));
        comm->Receive(robot_data.data(), robot_data.size());
        
        // Unmarshal robot info
        return UnmarshalRobotInfo(robot_data);
    }
};

#endif // CLIENT_STUB_H