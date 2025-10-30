// #ifndef __CLIENT_THREAD_H__
// #define __CLIENT_THREAD_H__

// #include <chrono>
// #include <ctime>
// #include <string>

// #include "ClientStub.h"
// #include "ClientTimer.h"

// class ClientThreadClass {
// 	int customer_id;
// 	int num_orders;
// 	int robot_type;
// 	ClientStub stub;

// 	ClientTimer timer;
// public:
// 	ClientThreadClass();
// 	void ThreadBody(std::string ip, int port, int id, int orders, int type);

// 	ClientTimer GetTimer();
// };


// #endif // end of #ifndef __CLIENT_THREAD_H__

#ifndef CLIENT_THREAD_H
#define CLIENT_THREAD_H

#include "ClientStub.h"
#include "ClientTimer.h"
#include "Messages.h"

class ClientThreadClass {
private:
    ClientStub stub;
    int customer_id;
    int num_orders;
    int request_type;
    ClientTimer timer;

public:
    ClientThreadClass();
    void ThreadBody(std::string ip, int port, int id, int orders, int type);
    ClientTimer GetTimer();
};

#endif