
// #ifndef SERVER_THREAD_H
// #define SERVER_THREAD_H

// #include <condition_variable>
// #include <future>
// #include <map>
// #include <mutex>
// #include <queue>
// #include <thread>
// #include <vector>

// #include "Messages.h"
// #include "ServerSocket.h"

// // State machine log entry
// struct MapOp {
//     int opcode;  // 1 = update value
//     int arg1;    // customer_id
//     int arg2;    // new value (order_number)
// };

// // Admin request structure
// struct AdminRequest {
//     CustomerRequest request;
//     int engineer_id;
//     std::promise<int> prom;  // Returns admin_id
// };

// class RobotFactory {
// private:
//     // Customer record map and state machine log
//     std::map<int, int> customer_record;
//     std::vector<MapOp> smr_log;
//     std::mutex record_lock;  // Protects both customer_record and smr_log

//     // Admin request queue
//     std::queue<std::unique_ptr<AdminRequest>> arq;  // admin request queue
//     std::mutex arq_lock;
//     std::condition_variable arq_cv;

//     RobotInfo CreateRobotAndUpdateRecord(CustomerRequest request, int engineer_id);
//     CustomerRecord ReadCustomerRecord(int customer_id);

// public:
//     void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
//     void AdminThread(int id);
// };

// #endif

#ifndef SERVER_THREAD_H
#define SERVER_THREAD_H

#include <condition_variable>
#include <future>
#include <map>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "ClientSocket.h"
#include "Messages.h"
#include "ServerSocket.h"

// State machine log entry
struct MapOp {
    int opcode;  // 1 = update value
    int arg1;    // customer_id
    int arg2;    // new value (order_number)
};

// Peer server info
struct PeerInfo {
    int id;
    std::string ip;
    int port;
};

// Admin request structure
struct AdminRequest {
    CustomerRequest request;
    int engineer_id;
    std::promise<int> prom;  // Returns admin_id
};

class RobotFactory {
private:
    // Factory identification
    int factory_id;
    int primary_id;
    std::vector<PeerInfo> peers;

    // Customer record map and state machine log
    std::map<int, int> customer_record;
    std::vector<MapOp> smr_log;
    int last_index;
    int committed_index;
    std::mutex record_lock;  // Protects customer_record, smr_log, last_index, committed_index

    // Admin request queue (PFA)
    std::queue<std::unique_ptr<AdminRequest>> arq;
    std::mutex arq_lock;
    std::condition_variable arq_cv;

    // Connections to backup servers
    std::vector<std::unique_ptr<ClientSocket>> backup_connections;
    bool connections_established;
    std::mutex connection_lock;

    RobotInfo CreateRobotAndUpdateRecord(CustomerRequest request, int engineer_id);
    CustomerRecord ReadCustomerRecord(int customer_id);
    void EstablishBackupConnections();
    bool ReplicateToBackup(int backup_index, MapOp& op);

public:
    RobotFactory(int id, std::vector<PeerInfo> peer_list);
    void EngineerThread(std::unique_ptr<ServerSocket> socket, int id);
    void AdminThread(int id);
    void IdleFactoryAdminThread(std::unique_ptr<ServerSocket> socket);
};

#endif