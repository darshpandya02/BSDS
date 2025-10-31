// #include <cstring>
// #include <iostream>
// #include <arpa/inet.h>
// #include "Messages.h"

// // CustomerRequest implementation
// CustomerRequest::CustomerRequest() {
//     customer_id = -1;
//     order_number = -1;
//     request_type = -1;
// }

// void CustomerRequest::SetRequest(int id, int number, int type) {
//     customer_id = id;
//     order_number = number;
//     request_type = type;
// }

// int CustomerRequest::GetCustomerId() { return customer_id; }
// int CustomerRequest::GetOrderNumber() { return order_number; }
// int CustomerRequest::GetRequestType() { return request_type; }

// int CustomerRequest::Size() {
//     return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type);
// }

// void CustomerRequest::Marshal(char *buffer) {
//     int net_customer_id = htonl(customer_id);
//     int net_order_number = htonl(order_number);
//     int net_request_type = htonl(request_type);
//     int offset = 0;
//     memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
//     offset += sizeof(net_order_number);
//     memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
// }

// void CustomerRequest::Unmarshal(char *buffer) {
//     int net_customer_id;
//     int net_order_number;
//     int net_request_type;
//     int offset = 0;
//     memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
//     offset += sizeof(net_order_number);
//     memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));

//     customer_id = ntohl(net_customer_id);
//     order_number = ntohl(net_order_number);
//     request_type = ntohl(net_request_type);
// }

// bool CustomerRequest::IsValid() {
//     return (customer_id != -1);
// }

// void CustomerRequest::Print() {
//     std::cout << "id " << customer_id << " ";
//     std::cout << "num " << order_number << " ";
//     std::cout << "type " << request_type << std::endl;
// }

// // RobotInfo implementation
// RobotInfo::RobotInfo() {
//     customer_id = -1;
//     order_number = -1;
//     request_type = -1;
//     engineer_id = -1;
//     admin_id = -1;
// }

// void RobotInfo::SetInfo(int id, int number, int type, int engid, int adminid) {
//     customer_id = id;
//     order_number = number;
//     request_type = type;
//     engineer_id = engid;
//     admin_id = adminid;
// }

// void RobotInfo::CopyRequest(CustomerRequest request) {
//     customer_id = request.GetCustomerId();
//     order_number = request.GetOrderNumber();
//     request_type = request.GetRequestType();
// }

// void RobotInfo::SetEngineerId(int id) { engineer_id = id; }
// void RobotInfo::SetAdminId(int id) { admin_id = id; }

// int RobotInfo::GetCustomerId() { return customer_id; }
// int RobotInfo::GetOrderNumber() { return order_number; }
// int RobotInfo::GetRequestType() { return request_type; }
// int RobotInfo::GetEngineerId() { return engineer_id; }
// int RobotInfo::GetAdminId() { return admin_id; }

// int RobotInfo::Size() {
//     return sizeof(customer_id) + sizeof(order_number) + sizeof(request_type)
//         + sizeof(engineer_id) + sizeof(admin_id);
// }

// void RobotInfo::Marshal(char *buffer) {
//     int net_customer_id = htonl(customer_id);
//     int net_order_number = htonl(order_number);
//     int net_request_type = htonl(request_type);
//     int net_engineer_id = htonl(engineer_id);
//     int net_admin_id = htonl(admin_id);
//     int offset = 0;

//     memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(buffer + offset, &net_order_number, sizeof(net_order_number));
//     offset += sizeof(net_order_number);
//     memcpy(buffer + offset, &net_request_type, sizeof(net_request_type));
//     offset += sizeof(net_request_type);
//     memcpy(buffer + offset, &net_engineer_id, sizeof(net_engineer_id));
//     offset += sizeof(net_engineer_id);
//     memcpy(buffer + offset, &net_admin_id, sizeof(net_admin_id));
// }

// void RobotInfo::Unmarshal(char *buffer) {
//     int net_customer_id;
//     int net_order_number;
//     int net_request_type;
//     int net_engineer_id;
//     int net_admin_id;
//     int offset = 0;

//     memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(&net_order_number, buffer + offset, sizeof(net_order_number));
//     offset += sizeof(net_order_number);
//     memcpy(&net_request_type, buffer + offset, sizeof(net_request_type));
//     offset += sizeof(net_request_type);
//     memcpy(&net_engineer_id, buffer + offset, sizeof(net_engineer_id));
//     offset += sizeof(net_engineer_id);
//     memcpy(&net_admin_id, buffer + offset, sizeof(net_admin_id));

//     customer_id = ntohl(net_customer_id);
//     order_number = ntohl(net_order_number);
//     request_type = ntohl(net_request_type);
//     engineer_id = ntohl(net_engineer_id);
//     admin_id = ntohl(net_admin_id);
// }

// bool RobotInfo::IsValid() {
//     return (customer_id != -1);
// }

// void RobotInfo::Print() {
//     std::cout << "id " << customer_id << " ";
//     std::cout << "num " << order_number << " ";
//     std::cout << "type " << request_type << " ";
//     std::cout << "engid " << engineer_id << " ";
//     std::cout << "adminid " << admin_id << std::endl;
// }

// // CustomerRecord implementation
// CustomerRecord::CustomerRecord() {
//     customer_id = -1;
//     last_order = -1;
// }

// void CustomerRecord::SetRecord(int id, int order) {
//     customer_id = id;
//     last_order = order;
// }

// int CustomerRecord::GetCustomerId() { return customer_id; }
// int CustomerRecord::GetLastOrder() { return last_order; }

// int CustomerRecord::Size() {
//     return sizeof(customer_id) + sizeof(last_order);
// }

// void CustomerRecord::Marshal(char *buffer) {
//     int net_customer_id = htonl(customer_id);
//     int net_last_order = htonl(last_order);
//     int offset = 0;
//     memcpy(buffer + offset, &net_customer_id, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(buffer + offset, &net_last_order, sizeof(net_last_order));
// }

// void CustomerRecord::Unmarshal(char *buffer) {
//     int net_customer_id;
//     int net_last_order;
//     int offset = 0;
//     memcpy(&net_customer_id, buffer + offset, sizeof(net_customer_id));
//     offset += sizeof(net_customer_id);
//     memcpy(&net_last_order, buffer + offset, sizeof(net_last_order));

//     customer_id = ntohl(net_customer_id);
//     last_order = ntohl(net_last_order);
// }

// bool CustomerRecord::IsValid() {
//     return (customer_id != -1);
// }

// void CustomerRecord::Print() {
//     std::cout << "customer_id: " << customer_id << "\t" << "last_order: " << last_order << std::endl;
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
    std::cout << customer_id << "\t" << last_order << std::endl;
}

// IdentificationMsg implementation
IdentificationMsg::IdentificationMsg() {
    id_type = -1;
}

void IdentificationMsg::SetIdType(int type) {
    id_type = type;
}

int IdentificationMsg::GetIdType() {
    return id_type;
}

int IdentificationMsg::Size() {
    return sizeof(id_type);
}

void IdentificationMsg::Marshal(char *buffer) {
    int net_id_type = htonl(id_type);
    memcpy(buffer, &net_id_type, sizeof(net_id_type));
}

void IdentificationMsg::Unmarshal(char *buffer) {
    int net_id_type;
    memcpy(&net_id_type, buffer, sizeof(net_id_type));
    id_type = ntohl(net_id_type);
}

// ReplicationRequest implementation
ReplicationRequest::ReplicationRequest() {
    factory_id = -1;
    committed_index = -1;
    last_index = -1;
    opcode = -1;
    arg1 = -1;
    arg2 = -1;
}

void ReplicationRequest::SetRequest(int fid, int cindex, int lindex, int op, int a1, int a2) {
    factory_id = fid;
    committed_index = cindex;
    last_index = lindex;
    opcode = op;
    arg1 = a1;
    arg2 = a2;
}

int ReplicationRequest::GetFactoryId() { return factory_id; }
int ReplicationRequest::GetCommittedIndex() { return committed_index; }
int ReplicationRequest::GetLastIndex() { return last_index; }
int ReplicationRequest::GetOpcode() { return opcode; }
int ReplicationRequest::GetArg1() { return arg1; }
int ReplicationRequest::GetArg2() { return arg2; }

int ReplicationRequest::Size() {
    return sizeof(factory_id) + sizeof(committed_index) + sizeof(last_index)
        + sizeof(opcode) + sizeof(arg1) + sizeof(arg2);
}

void ReplicationRequest::Marshal(char *buffer) {
    int net_factory_id = htonl(factory_id);
    int net_committed_index = htonl(committed_index);
    int net_last_index = htonl(last_index);
    int net_opcode = htonl(opcode);
    int net_arg1 = htonl(arg1);
    int net_arg2 = htonl(arg2);
    int offset = 0;

    memcpy(buffer + offset, &net_factory_id, sizeof(net_factory_id));
    offset += sizeof(net_factory_id);
    memcpy(buffer + offset, &net_committed_index, sizeof(net_committed_index));
    offset += sizeof(net_committed_index);
    memcpy(buffer + offset, &net_last_index, sizeof(net_last_index));
    offset += sizeof(net_last_index);
    memcpy(buffer + offset, &net_opcode, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(buffer + offset, &net_arg1, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(buffer + offset, &net_arg2, sizeof(net_arg2));
}

void ReplicationRequest::Unmarshal(char *buffer) {
    int net_factory_id;
    int net_committed_index;
    int net_last_index;
    int net_opcode;
    int net_arg1;
    int net_arg2;
    int offset = 0;

    memcpy(&net_factory_id, buffer + offset, sizeof(net_factory_id));
    offset += sizeof(net_factory_id);
    memcpy(&net_committed_index, buffer + offset, sizeof(net_committed_index));
    offset += sizeof(net_committed_index);
    memcpy(&net_last_index, buffer + offset, sizeof(net_last_index));
    offset += sizeof(net_last_index);
    memcpy(&net_opcode, buffer + offset, sizeof(net_opcode));
    offset += sizeof(net_opcode);
    memcpy(&net_arg1, buffer + offset, sizeof(net_arg1));
    offset += sizeof(net_arg1);
    memcpy(&net_arg2, buffer + offset, sizeof(net_arg2));

    factory_id = ntohl(net_factory_id);
    committed_index = ntohl(net_committed_index);
    last_index = ntohl(net_last_index);
    opcode = ntohl(net_opcode);
    arg1 = ntohl(net_arg1);
    arg2 = ntohl(net_arg2);
}

// ReplicationResponse implementation
ReplicationResponse::ReplicationResponse() {
    status = -1;
}

void ReplicationResponse::SetStatus(int s) {
    status = s;
}

int ReplicationResponse::GetStatus() {
    return status;
}

int ReplicationResponse::Size() {
    return sizeof(status);
}

void ReplicationResponse::Marshal(char *buffer) {
    int net_status = htonl(status);
    memcpy(buffer, &net_status, sizeof(net_status));
}

void ReplicationResponse::Unmarshal(char *buffer) {
    int net_status;
    memcpy(&net_status, buffer, sizeof(net_status));
    status = ntohl(net_status);
}