
// #ifndef SERVER_STUB_H
// #define SERVER_STUB_H

// #include <memory>

// #include "Messages.h"
// #include "ServerSocket.h"

// class ServerStub {
// private:
//     std::unique_ptr<ServerSocket> socket;

// public:
//     ServerStub();
//     void Init(std::unique_ptr<ServerSocket> socket);
//     CustomerRequest ReceiveRequest();
//     int ShipRobot(RobotInfo info);
//     int ReturnRecord(CustomerRecord record);
// };

// #endif

#ifndef SERVER_STUB_H
#define SERVER_STUB_H

#include <memory>

#include "Messages.h"
#include "ServerSocket.h"

class ServerStub {
private:
    std::unique_ptr<ServerSocket> socket;

public:
    ServerStub();
    void Init(std::unique_ptr<ServerSocket> socket);
    IdentificationMsg ReceiveIdentification();
    CustomerRequest ReceiveRequest();
    int ShipRobot(RobotInfo info);
    int ReturnRecord(CustomerRecord record);
    std::unique_ptr<ServerSocket> GetSocket();
};

#endif