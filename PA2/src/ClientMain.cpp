// #include <iostream> 

// int main(int argc, char *argv[]) {
// 	std::cout << "Hello world!" << std::endl;
// 	return 1;
// }

#include "ClientStub.h"
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <numeric>
#include <algorithm>
#include <iomanip>

// Thread-safe statistics collection
std::mutex stats_mutex;
std::vector<double> all_latencies; // in microseconds

class Customer {
private:
    int customer_id;
    std::string server_ip;
    int server_port;
    int num_orders;
    int robot_type;
    
public:
    Customer(int id, const std::string& ip, int port, int orders, int rtype)
        : customer_id(id), server_ip(ip), server_port(port), 
          num_orders(orders), robot_type(rtype) {}
    
    void Run() {
        try {
            // Each customer creates its own connection
            ClientStub client;
            client.Init(server_ip, server_port);
            
            std::vector<double> local_latencies;
            local_latencies.reserve(num_orders);
            
            // Place orders
            for (int i = 0; i < num_orders; i++) {
                Order order;
                order.customer_id = customer_id;
                order.order_number = i + 1;
                order.robot_type = robot_type;
                
                // Measure latency for this order
                auto start = std::chrono::high_resolution_clock::now();
                
                RobotInfo robot = client.Order(order);
                
                auto end = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
                    end - start).count();
                
                local_latencies.push_back(static_cast<double>(duration));
                
                // Verify the robot info matches the order
                if (robot.customer_id != order.customer_id || 
                    robot.order_number != order.order_number ||
                    robot.robot_type != order.robot_type) {
                    std::cerr << "Warning: Robot info mismatch for customer " 
                              << customer_id << "\n";
                }
            }
            
            // Add local latencies to global statistics
            {
                std::lock_guard<std::mutex> lock(stats_mutex);
                all_latencies.insert(all_latencies.end(), 
                                    local_latencies.begin(), 
                                    local_latencies.end());
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Customer " << customer_id << " error: " 
                      << e.what() << std::endl;
        }
    }
};

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name 
              << " [ip_addr] [port] [#customers] [#orders] [robot_type]\n";
    std::cout << "Example: " << program_name 
              << " 127.0.0.1 12345 16 1000 0\n";
    std::cout << "  ip_addr: Server IP address\n";
    std::cout << "  port: Server port number\n";
    std::cout << "  #customers: Number of concurrent customers\n";
    std::cout << "  #orders: Number of orders per customer\n";
    std::cout << "  robot_type: 0 for regular, 1 for special\n";
}

int main(int argc, char* argv[]) {
    if (argc != 6) {
        PrintUsage(argv[0]);
        return 1;
    }
    
    // Parse command line arguments
    std::string server_ip = argv[1];
    int server_port = std::stoi(argv[2]);
    int num_customers = std::stoi(argv[3]);
    int num_orders = std::stoi(argv[4]);
    int robot_type = std::stoi(argv[5]);
    
    // Validate inputs
    if (num_customers <= 0 || num_orders <= 0) {
        std::cerr << "Error: Number of customers and orders must be positive\n";
        return 1;
    }
    
    if (robot_type != 0 && robot_type != 1) {
        std::cerr << "Error: Robot type must be 0 (regular) or 1 (special)\n";
        return 1;
    }
    
    std::cout << "Starting client with parameters:\n";
    std::cout << "  Server: " << server_ip << ":" << server_port << "\n";
    std::cout << "  Customers: " << num_customers << "\n";
    std::cout << "  Orders per customer: " << num_orders << "\n";
    std::cout << "  Robot type: " << (robot_type == 0 ? "Regular" : "Special") << "\n";
    std::cout << "  Total orders: " << (num_customers * num_orders) << "\n\n";
    
    // Clear global statistics
    all_latencies.clear();
    all_latencies.reserve(num_customers * num_orders);
    
    // Start timing for throughput calculation
    auto total_start = std::chrono::high_resolution_clock::now();
    
    // Create customer threads
    std::vector<std::thread> customer_threads;
    customer_threads.reserve(num_customers);
    
    for (int i = 0; i < num_customers; i++) {
        int customer_id = 1000 + i; // Start customer IDs from 1000
        Customer customer(customer_id, server_ip, server_port, num_orders, robot_type);
        customer_threads.emplace_back(&Customer::Run, customer);
    }
    
    // Wait for all customers to complete
    for (auto& thread : customer_threads) {
        thread.join();
    }
    
    // End timing
    auto total_end = std::chrono::high_resolution_clock::now();
    auto total_duration = std::chrono::duration_cast<std::chrono::microseconds>(
        total_end - total_start).count();
    
    // Calculate statistics
    if (all_latencies.empty()) {
        std::cerr << "Error: No latency data collected\n";
        return 1;
    }
    
    double avg_latency = std::accumulate(all_latencies.begin(), all_latencies.end(), 0.0) 
                        / all_latencies.size();
    double min_latency = *std::min_element(all_latencies.begin(), all_latencies.end());
    double max_latency = *std::max_element(all_latencies.begin(), all_latencies.end());
    
    // Calculate throughput (orders per second)
    double total_seconds = total_duration / 1000000.0;
    double throughput = all_latencies.size() / total_seconds;
    
    // Print results in tab-separated format
    std::cout << "\n=== Performance Results ===\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << avg_latency << "\t" 
              << min_latency << "\t" 
              << max_latency << "\t" 
              << throughput << "\n";
    
    std::cout << "\nDetailed Statistics:\n";
    std::cout << "  Average Latency: " << avg_latency << " μs\n";
    std::cout << "  Minimum Latency: " << min_latency << " μs\n";
    std::cout << "  Maximum Latency: " << max_latency << " μs\n";
    std::cout << "  Throughput: " << throughput << " orders/sec\n";
    std::cout << "  Total Time: " << total_seconds << " seconds\n";
    std::cout << "  Total Orders: " << all_latencies.size() << "\n";
    
    return 0;
}

// Compile: g++ -std=c++11 -pthread -O2 -o client customer_client.cpp
// Run: ./client 127.0.0.1 12345 16 1000 0