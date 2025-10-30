// #ifndef __MESSAGES_H__
// #define __MESSAGES_H__

// #include <string>

// class RobotOrder {
// private:
// 	int customer_id;
// 	int order_number;
// 	int robot_type;

// public:
// 	RobotOrder();
// 	void operator = (const RobotOrder &order) {
// 		customer_id = order.customer_id;
// 		order_number = order.order_number;
// 		robot_type = order.robot_type;
// 	}
// 	void SetOrder(int cid, int order_num, int type);
// 	int GetCustomerId();
// 	int GetOrderNumber();
// 	int GetRobotType();

// 	int Size();

// 	void Marshal(char *buffer);
// 	void Unmarshal(char *buffer);

// 	bool IsValid();

// 	void Print();
// };

// class RobotInfo {
// private:
// 	int customer_id;
// 	int order_number;
// 	int robot_type;
// 	int engineer_id;
// 	int expert_id;

// public:
// 	RobotInfo();
// 	void operator = (const RobotInfo &info) {
// 		customer_id = info.customer_id;
// 		order_number = info.order_number;
// 		robot_type = info.robot_type;
// 		engineer_id = info.engineer_id;
// 		expert_id = info.expert_id;
// 	}
// 	void SetInfo(int cid, int order_num, int type, int engid, int expid);
// 	void CopyOrder(RobotOrder order);
// 	void SetEngineerId(int id);
// 	void SetExpertId(int id);

// 	int GetCustomerId();
// 	int GetOrderNumber();
// 	int GetRobotType();
// 	int GetEngineerId();
// 	int GetExpertId();

// 	int Size();

// 	void Marshal(char *buffer);
// 	void Unmarshal(char *buffer);

// 	bool IsValid();

// 	void Print();
// };

// #endif // #ifndef __MESSAGES_H__

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
    int admin_id;  // renamed from expert_id

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

// New: CustomerRecord for record read responses
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

#endif