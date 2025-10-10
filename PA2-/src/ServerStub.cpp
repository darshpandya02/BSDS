// ServerStub.cpp
#include "ServerStub.h"
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

ServerStub::ServerStub() : sockfd(-1) {}
ServerStub::~ServerStub() {
    if (sockfd >= 0) close(sockfd);
}
bool ServerStub::Init(int connected_sock) {
    sockfd = connected_sock;
    return true;
}
Order ServerStub::ReceiveOrder() {
    char order_buf[sizeof(Order)];
    recv(sockfd, order_buf, sizeof(order_buf), 0);
    Order order;
    order.deserialize(order_buf);
    return order;
}
bool ServerStub::ShipRobot(const RobotInfo& robot) {
    char robot_buf[sizeof(RobotInfo)];
    robot.serialize(robot_buf);
    return send(sockfd, robot_buf, sizeof(robot_buf), 0) == sizeof(robot_buf);
}
