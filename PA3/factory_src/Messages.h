// #ifndef MESSAGES_H
// #define MESSAGES_H

// #include <iostream>

// // CustomerRequest replaces RobotOrder
// class CustomerRequest {
// private:
//     int customer_id;
//     int order_number;
//     int request_type;  // 1 = robot order, 2 = record read

// public:
//     CustomerRequest();
//     void SetRequest(int id, int number, int type);
//     int GetCustomerId();
//     int GetOrderNumber();
//     int GetRequestType();
//     int Size();
//     void Marshal(char *buffer);
//     void Unmarshal(char *buffer);
//     bool IsValid();
//     void Print();
// };

// // RobotInfo stays similar but expert_id becomes admin_id
// class RobotInfo {
// private:
//     int customer_id;
//     int order_number;
//     int request_type;
//     int engineer_id;
//     int admin_id;  // renamed from expert_id

// public:
//     RobotInfo();
//     void SetInfo(int id, int number, int type, int engid, int adminid);
//     void CopyRequest(CustomerRequest request);
//     void SetEngineerId(int id);
//     void SetAdminId(int id);
//     int GetCustomerId();
//     int GetOrderNumber();
//     int GetRequestType();
//     int GetEngineerId();
//     int GetAdminId();
//     int Size();
//     void Marshal(char *buffer);
//     void Unmarshal(char *buffer);
//     bool IsValid();
//     void Print();
// };

// // CustomerRecord for record read responses
// class CustomerRecord {
// private:
//     int customer_id;
//     int last_order;

// public:
//     CustomerRecord();
//     void SetRecord(int id, int order);
//     int GetCustomerId();
//     int GetLastOrder();
//     int Size();
//     void Marshal(char *buffer);
//     void Unmarshal(char *buffer);
//     bool IsValid();
//     void Print();
// };

// #endif

#ifndef MESSAGES_H
#define MESSAGES_H

#include <iostream>

// CustomerRequest replaces RobotOrder
class CustomerRequest {
private:
    int customer_id;
    int order_number;
    int request_type;  // 1 = robot order, 2 = record read

public:
    CustomerRequest();
    void SetRequest(int id, int number, int type);
    int GetCustomerId();
    int GetOrderNumber();
    int GetRequestType();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
    bool IsValid();
    void Print();
};

// RobotInfo stays similar but expert_id becomes admin_id
class RobotInfo {
private:
    int customer_id;
    int order_number;
    int request_type;
    int engineer_id;
    int admin_id;

public:
    RobotInfo();
    void SetInfo(int id, int number, int type, int engid, int adminid);
    void CopyRequest(CustomerRequest request);
    void SetEngineerId(int id);
    void SetAdminId(int id);
    int GetCustomerId();
    int GetOrderNumber();
    int GetRequestType();
    int GetEngineerId();
    int GetAdminId();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
    bool IsValid();
    void Print();
};

// CustomerRecord for record read responses
class CustomerRecord {
private:
    int customer_id;
    int last_order;

public:
    CustomerRecord();
    void SetRecord(int id, int order);
    int GetCustomerId();
    int GetLastOrder();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
    bool IsValid();
    void Print();
};

// Identification message
class IdentificationMsg {
private:
    int id_type;  // 0 = customer, 1 = PFA

public:
    IdentificationMsg();
    void SetIdType(int type);
    int GetIdType();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
};

// Replication request from PFA to IFA
class ReplicationRequest {
private:
    int factory_id;
    int committed_index;
    int last_index;
    int opcode;
    int arg1;
    int arg2;

public:
    ReplicationRequest();
    void SetRequest(int fid, int cindex, int lindex, int op, int a1, int a2);
    int GetFactoryId();
    int GetCommittedIndex();
    int GetLastIndex();
    int GetOpcode();
    int GetArg1();
    int GetArg2();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
};

// Replication response from IFA to PFA
class ReplicationResponse {
private:
    int status;  // 0 = success

public:
    ReplicationResponse();
    void SetStatus(int s);
    int GetStatus();
    int Size();
    void Marshal(char *buffer);
    void Unmarshal(char *buffer);
};

#endif