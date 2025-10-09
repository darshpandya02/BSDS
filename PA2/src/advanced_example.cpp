#include "ClientStub.h"
#include "ServerStub.h"
#include "SocketComm.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

// Thread-safe console output
std::mutex cout_mutex;

void SafePrint(const std::string& message) {
    std::lock_guard<std::mutex> lock(cout_mutex);
    std::cout << message << std::flush;
}

// ============================================================================
// ADVANCED CLIENT EXAMPLE: Multiple Customers Placing Orders Concurrently
// ============================================================================

class Customer {
private:
    int customer_id;
    std::string server_ip;
    int server_port;
    
public:
    Customer(int id, const std::string& ip, int port)
        : customer_id(id), server_ip(ip), server_port(port) {}
    
    void PlaceOrders(int num_orders) {
        try {
            // Each customer has their own connection
            ClientStub client;
            client.Init(server_ip, server_port);
            
            SafePrint("[Customer " + std::to_string(customer_id) + 
                     "] Connected to factory\n");
            
            for (int i = 0; i < num_orders; i++) {
                // Create order
                Order order;
                order.customer_id = customer_id;
                order.order_number = i + 1;
                // Alternate between regular and special robots
                order.robot_type = (i % 2);
                
                SafePrint("[Customer " + std::to_string(customer_id) + 
                         "] Placing order #" + std::to_string(order.order_number) +
                         " (" + (order.robot_type == 0 ? "Regular" : "Special") + ")\n");
                
                // Place order and receive robot
                RobotInfo robot = client.Order(order);
                
                SafePrint("[Customer " + std::to_string(customer_id) + 
                         "] Received robot #" + std::to_string(robot.order_number) +
                         " (Engineer: " + std::to_string(robot.engineer_id) +
                         ", Expert: " + std::to_string(robot.expert_id) + ")\n");
                
                // Simulate time between orders
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
            
            SafePrint("[Customer " + std::to_string(customer_id) + 
                     "] All orders completed!\n");
            
        } catch (const std::exception& e) {
            SafePrint("[Customer " + std::to_string(customer_id) + 
                     "] Error: " + std::string(e.what()) + "\n");
        }
    }
};

void RunMultiCustomerDemo(const std::string& server_ip, int server_port) {
    std::cout << "\n=== MULTI-CUSTOMER DEMO ===\n";
    std::cout << "Simulating multiple customers placing orders concurrently...\n\n";
    
    std::vector<std::thread> customer_threads;
    
    // Create 3 customers, each placing 3 orders
    for (int i = 1; i <= 3; i++) {
        Customer customer(1000 + i, server_ip, server_port);
        customer_threads.emplace_back(&Customer::PlaceOrders, customer, 3);
    }
    
    // Wait for all customers to complete
    for (auto& thread : customer_threads) {
        thread.join();
    }
    
    std::cout << "\n=== ALL CUSTOMERS FINISHED ===\n\n";
}

// ============================================================================
// ADVANCED SERVER EXAMPLE: Factory with Multiple Assembly Lines
// ============================================================================

class RobotFactory {
private:
    int factory_id;
    std::vector<int> available_engineers;
    std::vector<int> available_experts;
    std::mutex engineer_mutex;
    std::mutex expert_mutex;
    int total_robots_built;
    std::mutex stats_mutex;
    
public:
    RobotFactory(int id) : factory_id(id), total_robots_built(0) {
        // Initialize available engineers (10 engineers)
        for (int i = 1; i <= 10; i++) {
            available_engineers.push_back(i);
        }
        
        // Initialize available experts (5 experts)
        for (int i = 1; i <= 5; i++) {
            available_experts.push_back(i);
        }
    }
    
    RobotInfo BuildRobot(const Order& order) {
        RobotInfo robot;
        robot.customer_id = order.customer_id;
        robot.order_number = order.order_number;
        robot.robot_type = order.robot_type;
        
        // Assign engineer
        {
            std::lock_guard<std::mutex> lock(engineer_mutex);
            if (!available_engineers.empty()) {
                robot.engineer_id = available_engineers.back();
                available_engineers.pop_back();
            } else {
                robot.engineer_id = 1; // Default engineer
            }
        }
        
        // Assign expert if special robot
        if (order.robot_type == 1) {
            std::lock_guard<std::mutex> lock(expert_mutex);
            if (!available_experts.empty()) {
                robot.expert_id = available_experts.back();
                available_experts.pop_back();
            } else {
                robot.expert_id = 1; // Default expert
            }
        } else {
            robot.expert_id = -1;
        }
        
        // Simulate assembly time
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        // Return resources
        {
            std::lock_guard<std::mutex> lock(engineer_mutex);
            available_engineers.push_back(robot.engineer_id);
        }
        
        if (robot.expert_id != -1) {
            std::lock_guard<std::mutex> lock(expert_mutex);
            available_experts.push_back(robot.expert_id);
        }
        
        // Update statistics
        {
            std::lock_guard<std::mutex> lock(stats_mutex);
            total_robots_built++;
        }
        
        return robot;
    }
    
    int GetTotalRobotsBuilt() {
        std::lock_guard<std::mutex> lock(stats_mutex);
        return total_robots_built;
    }
};

void HandleClientWithFactory(SocketComm* client_socket, int client_num, 
                             RobotFactory& factory) {
    try {
        SafePrint("[Factory Worker " + std::to_string(client_num) + 
                 "] Client connected\n");
        
        ServerStub server;
        server.Init(client_socket);
        
        while (true) {
            try {
                // Receive order
                Order order = server.ReceiveOrder();
                
                SafePrint("[Factory Worker " + std::to_string(client_num) + 
                         "] Order from customer " + std::to_string(order.customer_id) +
                         " (#" + std::to_string(order.order_number) + ")\n");
                
                // Build robot using factory
                RobotInfo robot = factory.BuildRobot(order);
                
                // Ship robot
                server.ShipRobot(robot);
                
                SafePrint("[Factory Worker " + std::to_string(client_num) + 
                         "] Robot shipped to customer " + 
                         std::to_string(robot.customer_id) + "\n");
                
            } catch (const std::exception&) {
                break; // Client disconnected
            }
        }
        
        SafePrint("[Factory Worker " + std::to_string(client_num) + 
                 "] Client disconnected\n");
        
    } catch (const std::exception& e) {
        SafePrint("[Factory Worker " + std::to_string(client_num) + 
                 "] Error: " + std::string(e.what()) + "\n");
    }
}

void RunAdvancedServer(int port) {
    try {
        RobotFactory factory(1);
        ServerSocket server;
        
        std::cout << "\n=== ADVANCED FACTORY SERVER ===\n";
        std::cout << "Starting factory on port " << port << "...\n";
        server.Listen(port);
        std::cout << "Factory ready for orders!\n\n";
        
        int worker_counter = 0;
        std::vector<std::thread> worker_threads;
        
        // Accept clients for a limited time (for demo purposes)
        auto start_time = std::chrono::steady_clock::now();
        
        while (true) {
            // Check if we should stop (after 5 seconds of demo)
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                current_time - start_time).count();
            
            if (elapsed > 5) {
                break;
            }
            
            try {
                SocketComm* client_socket = server.Accept();
                worker_counter++;
                
                std::thread worker(HandleClientWithFactory, client_socket, 
                                 worker_counter, std::ref(factory));
                worker_threads.push_back(std::move(worker));
                
            } catch (const std::exception& e) {
                SafePrint("Error accepting client: " + std::string(e.what()) + "\n");
            }
        }
        
        // Wait for all workers to finish
        for (auto& thread : worker_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        
        std::cout << "\n=== FACTORY STATISTICS ===\n";
        std::cout << "Total robots built: " << factory.GetTotalRobotsBuilt() << "\n";
        
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
    }
}

// ============================================================================
// MAIN: Choose demo mode
// ============================================================================

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "Usage:\n";
        std::cout << "  " << argv[0] << " server [port]          - Run advanced server\n";
        std::cout << "  " << argv[0] << " client [ip] [port]     - Run multi-customer demo\n";
        return 1;
    }
    
    std::string mode = argv[1];
    
    if (mode == "server") {
        int port = (argc >= 3) ? std::stoi(argv[2]) : 8888;
        RunAdvancedServer(port);
    }
    else if (mode == "client") {
        std::string ip = (argc >= 3) ? argv[2] : "127.0.0.1";
        int port = (argc >= 4) ? std::stoi(argv[3]) : 8888;
        RunMultiCustomerDemo(ip, port);
    }
    else {
        std::cerr << "Invalid mode. Use 'server' or 'client'\n";
        return 1;
    }
    
    return 0;
}

// Compile: g++ -std=c++11 -pthread -o advanced_demo advanced_example.cpp
// 
// Run server: ./advanced_demo server 8888
// Run client: ./advanced_demo client 127.0.0.1 8888