#include "ClientStub.h"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    std::string server_ip = "127.0.0.1";
    int server_port = 8888;
    
    // Override with command line arguments if provided
    if (argc >= 3) {
        server_ip = argv[1];
        server_port = std::stoi(argv[2]);
    }
    
    try {
        // Create and initialize client stub
        ClientStub client;
        std::cout << "Connecting to server at " << server_ip << ":" << server_port << "...\n";
        client.Init(server_ip, server_port);
        std::cout << "Connected successfully!\n\n";
        
        // Create and send multiple orders
        for (int i = 0; i < 3; i++) {
            // Create order
            Order order;
            order.customer_id = 1001;
            order.order_number = i + 1;
            order.robot_type = (i % 2); // Alternate between regular (0) and special (1)
            
            std::cout << "Sending Order #" << order.order_number << ":\n";
            std::cout << "  Customer ID: " << order.customer_id << "\n";
            std::cout << "  Robot Type: " << (order.robot_type == 0 ? "Regular" : "Special") << "\n";
            
            // Send order and receive robot
            RobotInfo robot = client.Order(order);
            
            std::cout << "Received Robot:\n";
            std::cout << "  Customer ID: " << robot.customer_id << "\n";
            std::cout << "  Order Number: " << robot.order_number << "\n";
            std::cout << "  Robot Type: " << (robot.robot_type == 0 ? "Regular" : "Special") << "\n";
            std::cout << "  Engineer ID: " << robot.engineer_id << "\n";
            std::cout << "  Expert ID: " << robot.expert_id;
            if (robot.expert_id == -1) {
                std::cout << " (No special module)";
            }
            std::cout << "\n\n";
        }
        
        std::cout << "All orders completed successfully!\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

// Compile: g++ -std=c++11 -o client test_client.cpp
// Run: ./client [server_ip] [server_port]
// Example: ./client 127.0.0.1 8888