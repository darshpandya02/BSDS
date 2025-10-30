// #include <cstring>
// #include <iostream>

// #include <arpa/inet.h>
// #include "Messages.h"

// RobotOrder::RobotOrder() {
// 	customer_id = -1;
// 	order_number = -1;
// 	robot_type = -1;
// }

// void RobotOrder::SetOrder(int id, int number, int type) {
// 	customer_id = id;
// 	order_number = number;
// 	robot_type = type;
// }

// int RobotOrder::GetCustomerId() { return customer_id; }
// int RobotOrder::GetOrderNumber() { return order_number; }
// int RobotOrder::GetRobotType() { return robot_type; }

// int RobotOrder::Size() {
// 	return sizeof(customer_id) + sizeof(order_number) + sizeof(robot_type);
// }

// void RobotOrder::Marshal(char *buffer) {
// 	int net_customer_id = htonl(customer_id);
// 	int net_order_number = htonl(order_number);
// 	int net_robot_type = htonl(robot_type);
// 	int offset = 0;
// 	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
// 	offset += sizeof(net_customer_id);
// 	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
// 	offset += sizeof(net_order_number);
// 	memcpy(buffer + offset, &net_robot_type, sizeof(net_robot_type));
// }

// void RobotOrder::Unmarshal(char *buffer) {
// 	int net_customer_id;
// 	int net_order_number;
// 	int net_robot_type;
// 	int offset = 0;
// 	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
// 	offset += sizeof(net_customer_id);
// 	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
// 	offset += sizeof(net_order_number);
// 	memcpy(&net_robot_type, buffer + offset, sizeof(net_robot_type));

// 	customer_id = ntohl(net_customer_id);
// 	order_number = ntohl(net_order_number);
// 	robot_type = ntohl(net_robot_type);
// }

// bool RobotOrder::IsValid() {
// 	return (customer_id != -1);
// }

// void RobotOrder::Print() {
// 	std::cout << "id " << customer_id << " ";
// 	std::cout << "num " << order_number << " ";
// 	std::cout << "type " << robot_type << std::endl;
// }

// RobotInfo::RobotInfo() {
// 	customer_id = -1;
// 	order_number = -1;
// 	robot_type = -1;
// 	engineer_id = -1;
// 	expert_id = -1;
// }

// void RobotInfo::SetInfo(int id, int number, int type, int engid, int expid) {
// 	customer_id = id;
// 	order_number = number;
// 	robot_type = type;
// 	engineer_id = engid;
// 	expert_id = expid;
// }

// void RobotInfo::CopyOrder(RobotOrder order) {
// 	customer_id = order.GetCustomerId();
// 	order_number = order.GetOrderNumber();
// 	robot_type = order.GetRobotType();
// }
// void RobotInfo::SetEngineerId(int id) { engineer_id = id; }
// void RobotInfo::SetExpertId(int id) { expert_id = id; }

// int RobotInfo::GetCustomerId() { return customer_id; }
// int RobotInfo::GetOrderNumber() { return order_number; }
// int RobotInfo::GetRobotType() { return robot_type; }
// int RobotInfo::GetEngineerId() { return engineer_id; }
// int RobotInfo::GetExpertId() { return expert_id; }

// int RobotInfo::Size() {
// 	return sizeof(customer_id) + sizeof(order_number) + sizeof(robot_type)
// 		+ sizeof(engineer_id) + sizeof(expert_id);
// }

// void RobotInfo::Marshal(char *buffer) {
// 	int net_customer_id = htonl(customer_id);
// 	int net_order_number = htonl(order_number);
// 	int net_robot_type = htonl(robot_type);
// 	int net_engineer_id = htonl(engineer_id);
// 	int net_expert_id = htonl(expert_id);
// 	int offset = 0;

// 	memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
// 	offset += sizeof(net_customer_id);
// 	memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
// 	offset += sizeof(net_order_number);
// 	memcpy(buffer + offset, &net_robot_type, sizeof(net_robot_type));
// 	offset += sizeof(net_robot_type);
// 	memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
// 	offset += sizeof(net_engineer_id);
// 	memcpy(buffer + offset, &net_expert_id, sizeof(net_expert_id));

// }

// void RobotInfo::Unmarshal(char *buffer) {
// 	int net_customer_id;
// 	int net_order_number;
// 	int net_robot_type;
// 	int net_engineer_id;
// 	int net_expert_id;
// 	int offset = 0;

// 	memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
// 	offset += sizeof(net_customer_id);
// 	memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
// 	offset += sizeof(net_order_number);
// 	memcpy(&net_robot_type, buffer + offset, sizeof(net_robot_type));
// 	offset += sizeof(net_robot_type);
// 	memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
// 	offset += sizeof(net_engineer_id);
// 	memcpy(&net_expert_id, buffer + offset, sizeof(net_expert_id));

// 	customer_id = ntohl(net_customer_id);
// 	order_number = ntohl(net_order_number);
// 	robot_type = ntohl(net_robot_type);
// 	engineer_id = ntohl(net_engineer_id);
// 	expert_id = ntohl(net_expert_id);
// }

// bool RobotInfo::IsValid() {
// 	return (customer_id != -1);
// }

// void RobotInfo::Print() {
// 	std::cout << "id " << customer_id << " ";
// 	std::cout << "num " << order_number << " ";
// 	std::cout << "type " << robot_type << " ";
// 	std::cout << "engid " << engineer_id << " ";
// 	std::cout << "expid " << expert_id << std::endl;
// }

#include <cstring>
#include <iostream>
#include <arpa/inet.h>
#include "Messages.h"

// CustomerRequest implementation
CustomerRequest::CustomerRequest() {
    customer_id = -1;
    order_number = -1;
    request_type = -1;
}

void CustomerRequest::SetRequest(int id, int number, int type) {
    customer_id = id;
    order_number = number;
    request_type = type;
}

int CustomerRequest::GetCustomerId() { return customer_id; }
int CustomerRequest::GetOrderNumber() { return order_number; }
int CustomerRequest::GetRequestType() { return request_type; }

int CustomerRequest::Size() {
    return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type);
}

void CustomerRequest::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_order_number = htonl(order_number);
    int net_request_type = htonl(request_type);
    int offset = 0;
    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
}

void CustomerRequest::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_order_number;
    int net_request_type;
    int offset = 0;
    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));

    customer_id = ntohl(net_customer_id);
    order_number = ntohl(net_order_number);
    request_type = ntohl(net_request_type);
}

bool CustomerRequest::IsValid() {
    return (customer_id != -1);
}

void CustomerRequest::Print() {
    std::cout << "id " << customer_id << " ";
    std::cout << "num " << order_number << " ";
    std::cout << "type " << request_type << std::endl;
}

// RobotInfo implementation
RobotInfo::RobotInfo() {
    customer_id = -1;
    order_number = -1;
    request_type = -1;
    engineer_id = -1;
    admin_id = -1;
}

void RobotInfo::SetInfo(int id, int number, int type, int engid, int adminid) {
    customer_id = id;
    order_number = number;
    request_type = type;
    engineer_id = engid;
    admin_id = adminid;
}

void RobotInfo::CopyRequest(CustomerRequest request) {
    customer_id = request.GetCustomerId();
    order_number = request.GetOrderNumber();
    request_type = request.GetRequestType();
}

void RobotInfo::SetEngineerId(int id) { engineer_id = id; }
void RobotInfo::SetAdminId(int id) { admin_id = id; }

int RobotInfo::GetCustomerId() { return customer_id; }
int RobotInfo::GetOrderNumber() { return order_number; }
int RobotInfo::GetRequestType() { return request_type; }
int RobotInfo::GetEngineerId() { return engineer_id; }
int RobotInfo::GetAdminId() { return admin_id; }

int RobotInfo::Size() {
    return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
        + sizeof(engineer_id) + sizeof(admin_id);
}

void RobotInfo::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_order_number = htonl(order_number);
    int net_request_type = htonl(request_type);
    int net_engineer_id = htonl(engineer_id);
    int net_admin_id = htonl(admin_id);
    int offset = 0;

    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
    offset += sizeof(net_request_type);
    memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
    offset += sizeof(net_engineer_id);
    memcpy(buffer + offset, &net_admin_id, sizeof(net_admin_id));
}

void RobotInfo::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_order_number;
    int net_request_type;
    int net_engineer_id;
    int net_admin_id;
    int offset = 0;

    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
    offset += sizeof(net_order_number);
    memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));
    offset += sizeof(net_request_type);
    memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
    offset += sizeof(net_engineer_id);
    memcpy(&net_admin_id, buffer + offset, sizeof(net_admin_id));

    customer_id = ntohl(net_customer_id);
    order_number = ntohl(net_order_number);
    request_type = ntohl(net_request_type);
    engineer_id = ntohl(net_engineer_id);
    admin_id = ntohl(net_admin_id);
}

bool RobotInfo::IsValid() {
    return (customer_id != -1);
}

void RobotInfo::Print() {
    std::cout << "id " << customer_id << " ";
    std::cout << "num " << order_number << " ";
    std::cout << "type " << request_type << " ";
    std::cout << "engid " << engineer_id << " ";
    std::cout << "adminid " << admin_id << std::endl;
}

// CustomerRecord implementation
CustomerRecord::CustomerRecord() {
    customer_id = -1;
    last_order = -1;
}

void CustomerRecord::SetRecord(int id, int order) {
    customer_id = id;
    last_order = order;
}

int CustomerRecord::GetCustomerId() { return customer_id; }
int CustomerRecord::GetLastOrder() { return last_order; }

int CustomerRecord::Size() {
    return sizeof(customer_id) + sizeof(last_order);
}

void CustomerRecord::Marshal(char *buffer) {
    int net_customer_id = htonl(customer_id);
    int net_last_order = htonl(last_order);
    int offset = 0;
    memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(buffer + offset, &net_last_order, sizeof(net_last_order));
}

void CustomerRecord::Unmarshal(char *buffer) {
    int net_customer_id;
    int net_last_order;
    int offset = 0;
    memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
    offset += sizeof(net_customer_id);
    memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));

    customer_id = ntohl(net_customer_id);
    last_order = ntohl(net_last_order);
}

bool CustomerRecord::IsValid() {
    return (customer_id != -1);
}

void CustomerRecord::Print() {
    std::cout << "customer_id: " << customer_id << "\t" << "last_order: " << last_order << std::endl;
}