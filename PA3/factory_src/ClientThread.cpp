#include "ClientThread.h"
#include "Messages.h"

#include <iostream>

ClientThreadClass::ClientThreadClass() {}

void ClientThreadClass::ThreadBody(std::string ip, int port, int id, int orders, int type) {
    customer_id = id;
    num_orders = orders;
    request_type = type;
    
    if (!stub.Init(ip, port)) {
        std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
        return;
    }

    if (request_type == 3) {
        // Special case: scan and print all customer records
        for (int i = 0; i <= num_orders; i++) {
            CustomerRequest request;
            CustomerRecord record;
            request.SetRequest(i, -1, 2);
            
            record = stub.ReadRecord(request);
            
            // Check if socket failed
            if (!record.IsValid()) {
                // Server disconnected - terminate gracefully
                return;
            }
            
            if (record.GetCustomerId() != -1) {
                record.Print();
            }
        }
    } else if (request_type == 1) {
        // Robot order requests
        for (int i = 0; i < num_orders; i++) {
            CustomerRequest request;
            RobotInfo robot;
            request.SetRequest(customer_id, i, 1);

            timer.Start();
            robot = stub.Order(request);
            timer.EndAndMerge();

            if (!robot.IsValid()) {
                // Server disconnected - terminate gracefully
                return;
            }
        }
    } else if (request_type == 2) {
        // Customer record read requests
        for (int i = 0; i < num_orders; i++) {
            CustomerRequest request;
            CustomerRecord record;
            request.SetRequest(customer_id, -1, 2);

            timer.Start();
            record = stub.ReadRecord(request);
            timer.EndAndMerge();

            if (!record.IsValid()) {
                // Server disconnected - terminate gracefully
                return;
            }
            
            if (record.GetCustomerId() != -1) {
                record.Print();
            }
        }
    }
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}