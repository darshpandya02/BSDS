// ClientStub.cpp
#include "ClientStub.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

ClientStub::ClientStub() : sockfd(-1) {}
ClientStub::~ClientStub() {
    if (sockfd >= 0) close(sockfd);
}
bool ClientStub::Init(const std::string& ip, int port) {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) return false;
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) return false;
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) return false;
    return true;
}

RobotInfo ClientStub::OrderRobot(const Order &order) {
    char buf[sizeof(Order)];
    order.serialize(buf);
    send(sockfd, buf, sizeof(buf), 0);

    char robot_buf[sizeof(RobotInfo)];
    recv(sockfd, robot_buf, sizeof(robot_buf), 0);
    RobotInfo robot;
    robot.deserialize(robot_buf);
    return robot;
}
