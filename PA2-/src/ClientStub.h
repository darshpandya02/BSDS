// ClientStub.h
#pragma once
#include "Order.h"
#include "RobotInfo.h"
#include <string>

class ClientStub {
public:
    ClientStub();
    ~ClientStub();
    bool Init(const std::string& ip, int port);
    RobotInfo OrderRobot(const Order &order);
private:
    int sockfd;
};
