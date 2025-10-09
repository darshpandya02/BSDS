#include "ServerStub.h"
#include "SocketComm.h"
#include <iostream>
#include <thread>
#include <random>

// Simulates robot assembly process
RobotInfo AssembleRobot(const Order& order) {
    RobotInfo robot;
    
    // Copy order information
    robot.customer_id = order.customer_id;
    robot.order_number = order.order_number;
    robot.robot_type = order.robot_type;
    
    // Simulate engineer assignment (random engineer ID between 1-10)
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> eng_dist(1, 10);
    robot.engineer_id = eng_dist(gen);
    
    // Assign expert for special robots
    if (order.robot_type == 1) { // Special robot
        static std::uniform_int_distribution<> exp_dist(1, 5);
        robot.expert_id = exp_dist(gen);
    } else {
        robot.expert_id = -1; // No special module
    }
    
    // Simulate assembly time
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    return robot;
}

// Handle individual client connection
void HandleClient(SocketComm* client_socket, int client_num) {
    try {
        std::cout << "[Client " << client_num << "] Connected\n";
        
        // Create server stub for this client
        ServerStub server;
        server.Init(client_socket);
        
        // Process orders from this client
        while (true) {
            try {
                // Receive order
                Order order = server.ReceiveOrder();
                
                std::cout << "[Client " << client_num << "] Received Order:\n";
                std::cout << "  Customer ID: " << order.customer_id << "\n";
                std::cout << "  Order Number: " << order.order_number << "\n";
                std::cout << "  Robot Type: " << (order.robot_type == 0 ? "Regular" : "Special") << "\n";
                
                // Assemble robot
                RobotInfo robot = AssembleRobot(order);
                
                std::cout << "[Client " << client_num << "] Assembled Robot:\n";
                std::cout << "  Engineer ID: " << robot.engineer_id << "\n";
                std::cout << "  Expert ID: " << robot.expert_id << "\n";
                
                // Ship robot back to client
                server.ShipRobot(robot);
                
                std::cout << "[Client " << client_num << "] Robot shipped\n\n";
                
            } catch (const std::exception& e) {
                // Client likely disconnected
                break;
            }
        }
        
        std::cout << "[Client " << client_num << "] Disconnected\n";
        
    } catch (const std::exception& e) {
        std::cerr << "[Client " << client_num << "] Error: " << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int port = 8888;
    
    // Override with command line argument if provided
    if (argc >= 2) {
        port = std::stoi(argv[1]);
    }
    
    try {
        ServerSocket server;
        std::cout << "Starting server on port " << port << "...\n";
        server.Listen(port);
        std::cout << "Server listening for connections...\n\n";
        
        int client_counter = 0;
        
        // Accept and handle client connections
        while (true) {
            try {
                // Accept new client connection
                SocketComm* client_socket = server.Accept();
                client_counter++;
                
                // Handle client in a new thread
                std::thread client_thread(HandleClient, client_socket, client_counter);
                client_thread.detach(); // Detach thread to handle multiple clients concurrently
                
            } catch (const std::exception& e) {
                std::cerr << "Error accepting client: " << e.what() << std::endl;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

// Compile: g++ -std=c++11 -pthread -o server test_server.cpp
// Run: ./server [port]
// Example: ./server 8888