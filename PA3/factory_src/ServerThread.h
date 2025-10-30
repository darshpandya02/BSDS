// #ifndef __SERVERTHREAD_H__
// #define __SERVERTHREAD_H__

// #include <condition_variable>
// #include <future>
// #include <mutex>
// #include <queue>
// #include <thread>

// #include "Messages.h"
// #include "ServerSocket.h"

// struct ExpertRequest {
// 	RobotInfo robot;
// 	std::promise<RobotInfo> prom;
// };

// class RobotFactory {
// private:
// 	std::queue<std::unique_ptr<ExpertRequest>> erq;
// 	std::mutex erq_lock;
// 	std::condition_variable erq_cv;

// 	RobotInfo CreateRegularRobot(RobotOrder order, int engineer_id);
// 	RobotInfo CreateSpecialRobot(RobotOrder order, int engineer_id);
// public:
// 	void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
// 	void ExpertThread(int id);
// };

// #endif // end of #ifndef __SERVERTHREAD_H__

#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include <condition_variable>
#include <future>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "Messages.h"
#include "ServerSocket.h"

// State machine log entry
struct MapOp {
    int opcode;  // 1 = update value
    int arg1;    // customer_id
    int arg2;    // new value (order_number)
};

// Admin request structure
struct AdminRequest {
    CustomerRequest request;
    int engineer_id;
    std::promise<int> prom;  // Returns admin_id
};

class RobotFactory {
private:
    // Customer record map and state machine log
    std::map<int, int> customer_record;
    std::vector<MapOp> smr_log;
    std::mutex record_lock;  // Protects both customer_record and smr_log

    // Admin request queue
    std::queue<std::unique_ptr<AdminRequest>> arq;  // admin request queue
    std::mutex arq_lock;
    std::condition_variable arq_cv;

    RobotInfo CreateRobotAndUpdateRecord(CustomerRequest request, int engineer_id);
    CustomerRecord ReadCustomerRecord(int customer_id);

public:
    void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
    void AdminThread(int id);
};

#endif