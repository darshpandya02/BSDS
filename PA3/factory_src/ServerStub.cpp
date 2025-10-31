// #include "ServerStub.h"

// ServerStub::ServerStub() {}

// void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
//     this->socket = std::move(socket);
// }

// CustomerRequest ServerStub::ReceiveRequest() {
//     char buffer[32];
//     CustomerRequest request;
//     if (socket->Recv(buffer, request.Size(), 0)) {
//         request.Unmarshal(buffer);
//     }
//     return request;
// }

// int ServerStub::ShipRobot(RobotInfo info) {
//     char buffer[32];
//     info.Marshal(buffer);
//     return socket->Send(buffer, info.Size(), 0);
// }

// int ServerStub::ReturnRecord(CustomerRecord record) {
//     char buffer[32];
//     record.Marshal(buffer);
//     return socket->Send(buffer, record.Size(), 0);
// }

#include "ServerStub.h"

ServerStub::ServerStub() {}

void ServerStub::Init(std::unique_ptr<ServerSocket> socket) {
    this->socket = std::move(socket);
}

IdentificationMsg ServerStub::ReceiveIdentification() {
    char buffer[32];
    IdentificationMsg msg;
    if (socket->Recv(buffer, msg.Size(), 0)) {
        msg.Unmarshal(buffer);
    }
    return msg;
}

CustomerRequest ServerStub::ReceiveRequest() {
    char buffer[32];
    CustomerRequest request;
    if (socket->Recv(buffer, request.Size(), 0)) {
        request.Unmarshal(buffer);
    }
    return request;
}

int ServerStub::ShipRobot(RobotInfo info) {
    char buffer[32];
    info.Marshal(buffer);
    return socket->Send(buffer, info.Size(), 0);
}

int ServerStub::ReturnRecord(CustomerRecord record) {
    char buffer[32];
    record.Marshal(buffer);
    return socket->Send(buffer, record.Size(), 0);
}

std::unique_ptr<ServerSocket> ServerStub::GetSocket() {
    return std::move(socket);
}