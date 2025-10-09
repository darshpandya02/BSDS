// #include <iostream>

// int main(int argc, char *argv[]) {
// 	std::cout << "Hello world!" << std::endl;
// 	return 0;
// }

#include "ServerStub.h"
#include "SocketComm.h"
#include "ExpertQueue.h"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <atomic>
#include <csignal>

// Global variables for graceful shutdown
std::atomic<bool> server_running(true);
ExpertQueue* global_expert_queue = nullptr;

void SignalHandler(int signal) {
    if (signal == SIGINT) {
        std::cout << "\n\nShutting down server...\n";
        server_running = false;
        if (global_expert_queue) {
            global_expert_queue->Shutdown();
        }
    }
}

// Expert engineer thread function
void ExpertEngineerThread(int expert_id, ExpertQueue* queue) {
    std::cout << "[Expert " << expert_id << "] Started\n";
    
    while (server_running) {
        ExpertRequest request(RobotInfo()); // Placeholder
        
        if (!queue->DequeueRequest(request)) {
            break; // Shutdown signal received
        }
        
        // Simulate work on special module (at least 100 microseconds)
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        
        // Add expert ID to robot info
        request.robot.expert_id = expert_id;
        
        // Notify the regular engineer that the work is done
        request.completion_promise.set_value(request.robot);
    }
    
    std::cout << "[Expert " << expert_id << "] Terminated\n";
}

// Regular engineer thread function
void EngineerThread(int engineer_id, SocketComm* client_socket, 
                   ExpertQueue* expert_queue, bool has_experts) {
    try {
        std::cout << "[Engineer " << engineer_id << "] Assigned to customer\n";
        
        ServerStub server;
        server.Init(client_socket);
        
        int orders_processed = 0;
        
        while (server_running) {
            try {
                // Receive order from customer
                Order order = server.ReceiveOrder();
                orders_processed++;
                
                // Create basic robot info
                RobotInfo robot;
                robot.customer_id = order.customer_id;
                robot.order_number = order.order_number;
                robot.robot_type = order.robot_type;
                robot.engineer_id = engineer_id;
                robot.expert_id = -1; // Default: no expert
                
                // Check if special robot and experts are available
                if (order.robot_type == 1 && has_experts && expert_queue) {
                    // Send request to expert engineer
                    std::future<RobotInfo> future = expert_queue->EnqueueRequest(robot);
                    
                    // Wait for expert to complete the work
                    robot = future.get();
                }
                
                // Ship robot back to customer
                server.ShipRobot(robot);
                
            } catch (const std::exception& e) {
                // Client likely disconnected (recv returns -1)
                break;
            }
        }
        
        std::cout << "[Engineer " << engineer_id << "] Customer disconnected. "
                  << "Processed " << orders_processed << " orders.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "[Engineer " << engineer_id << "] Error: " 
                  << e.what() << std::endl;
    }
}

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " [port] [#experts]\n";
    std::cout << "  port: Port number to listen on (10000-65535)\n";
    std::cout << "  #experts: Number of expert engineers (optional, default: 0)\n";
    std::cout << "Example: " << program_name << " 12345 2\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        PrintUsage(argv[0]);
        return 1;
    }
    
    // Parse command line arguments
    int port = std::stoi(argv[1]);
    int num_experts = (argc == 3) ? std::stoi(argv[2]) : 0;
    
    // Validate inputs
    if (port < 10000 || port > 65535) {
        std::cerr << "Error: Port must be between 10000 and 65535\n";
        return 1;
    }
    
    if (num_experts < 0) {
        std::cerr << "Error: Number of experts must be non-negative\n";
        return 1;
    }
    
    // Setup signal handler for graceful shutdown
    signal(SIGINT, SignalHandler);
    
    try {
        ServerSocket server;
        
        std::cout << "=== Robot Factory Server ===\n";
        std::cout << "Port: " << port << "\n";
        std::cout << "Expert Engineers: " << num_experts << "\n\n";
        
        // Create expert queue and thread pool if needed
        ExpertQueue expert_queue;
        global_expert_queue = &expert_queue;
        std::vector<std::thread> expert_threads;
        
        if (num_experts > 0) {
            std::cout << "Starting expert engineer thread pool...\n";
            for (int i = 0; i < num_experts; i++) {
                int expert_id = 10000 + i; // Use high IDs for experts
                expert_threads.emplace_back(ExpertEngineerThread, expert_id, &expert_queue);
            }
            std::cout << "Expert engineers ready.\n\n";
        }
        
        // Start listening for connections
        server.Listen(port);
        std::cout << "Factory listening for customers on port " << port << "...\n";
        std::cout << "Press Ctrl+C to shutdown.\n\n";
        
        int engineer_counter = 0;
        std::vector<std::thread> engineer_threads;
        
        // Accept clients in a loop
        while (server_running) {
            try {
                SocketComm* client_socket = server.Accept();
                
                if (!server_running) {
                    delete client_socket;
                    break;
                }
                
                engineer_counter++;
                int engineer_id = engineer_counter;
                
                // Create engineer thread for this customer
                std::thread engineer(EngineerThread, engineer_id, client_socket,
                                   (num_experts > 0) ? &expert_queue : nullptr,
                                   num_experts > 0);
                engineer_threads.push_back(std::move(engineer));
                
            } catch (const std::exception& e) {
                if (server_running) {
                    std::cerr << "Error accepting client: " << e.what() << std::endl;
                }
            }
        }
        
        std::cout << "\nWaiting for engineer threads to finish...\n";
        
        // Wait for all engineer threads to complete
        for (auto& thread : engineer_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        // Shutdown expert thread pool
        if (num_experts > 0) {
            std::cout << "Shutting down expert thread pool...\n";
            expert_queue.Shutdown();
            for (auto& thread : expert_threads) {
                if (thread.joinable()) {
                    thread.join();
                }
            }
        }
        
        std::cout << "Server shutdown complete.\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}

// Compile: g++ -std=c++11 -pthread -O2 -o server factory_server.cpp
// Run: ./server 12345 2