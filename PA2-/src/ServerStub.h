// ServerStub.h
#pragma once
#include "Order.h"
#include "RobotInfo.h"

class ServerStub {
public:
    ServerStub();
    ~ServerStub();
    bool Init(int connected_sock);
    Order ReceiveOrder();
    bool ShipRobot(const RobotInfo& robot);
private:
    int sockfd;
};
