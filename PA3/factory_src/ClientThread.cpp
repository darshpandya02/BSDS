// #include "ClientThread.h"
// #include "Messages.h"

// #include <iostream>

// ClientThreadClass::ClientThreadClass() {}

// void ClientThreadClass::ThreadBody(std::string ip, int port, int id, int orders, int type) {
// 	customer_id = id;
// 	num_orders = orders;
// 	robot_type = type;
// 	if (!stub.Init(ip, port)) {
// 		std::cout << "Thread " << customer_id << " failed to connect" << std::endl;
// 		return;
// 	}
// 	for (int i = 0; i < num_orders; i++) {
// 		RobotOrder order;
// 		RobotInfo robot;
// 		order.SetOrder(customer_id, i, robot_type);

// 		timer.Start();
// 		robot = stub.OrderRobot(order);
// 		timer.EndAndMerge();

// 		if (!robot.IsValid()) {
// 			std::cout << "Invalid robot " << customer_id << std::endl;
// 			break;	
// 		} 
// 	}
// }

// ClientTimer ClientThreadClass::GetTimer() {
// 	return timer;	
// }

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
        // num_orders represents the maximum customer_id to scan
        for (int i = 0; i <= num_orders; i++) {  // Changed to <= to include num_orders
            CustomerRequest request;
            CustomerRecord record;
            request.SetRequest(i, -1, 2);  // Read request with customer_id = i
            
            record = stub.ReadRecord(request);
            
            // Only print if record exists (customer_id != -1)
            if (record.IsValid() && record.GetCustomerId() != -1) {
                record.Print();
            }
        }
        // Don't use timer for request_type 3
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
                std::cout << "Invalid robot " << customer_id << std::endl;
                break;
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

            if (record.IsValid()) {
                record.Print();
            }
        }
    }
}

ClientTimer ClientThreadClass::GetTimer() {
    return timer;
}