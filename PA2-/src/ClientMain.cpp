#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "ClientStub.h"

struct CustomerData {
    int customer_id;
    int num_orders;
    int robot_type;
    std::vector<long long> latencies; // in microseconds
};

void customer_function(int customer_id, int num_orders, int robot_type, const std::string& ip, int port, CustomerData& data) {
    ClientStub stub;
    stub.Init(ip, port);
    for (int i = 0; i < num_orders; ++i) {
        Order order = {customer_id, i+1, robot_type};
        auto start_time = std::chrono::high_resolution_clock::now();
        RobotInfo robot = stub.OrderRobot(order);
        auto end_time = std::chrono::high_resolution_clock::now();
        auto latency = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        data.latencies.push_back(latency);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        std::cerr << "Usage: ./client ip port #customers #orders robot_type\n";
        return 1;
    }
    std::string ip = argv[1];
    int port = std::stoi(argv[2]);
    int num_customers = std::stoi(argv[3]);
    int num_orders = std::stoi(argv[4]);
    int robot_type = std::stoi(argv[5]);

    std::vector<std::thread> threads;
    std::vector<CustomerData> customer_data(num_customers);
    
    // Launch customer threads
    for (int i = 0; i < num_customers; ++i) {
        customer_data[i].customer_id = i + 1;
        customer_data[i].num_orders = num_orders;
        customer_data[i].robot_type = robot_type;
        threads.emplace_back(customer_function, i+1, num_orders, robot_type, ip, port, std::ref(customer_data[i]));
    }
    auto start_time = std::chrono::high_resolution_clock::now();

    // Join all threads
    for (auto& t : threads) t.join();

    auto end_time = std::chrono::high_resolution_clock::now();

    // Gather latencies
    std::vector<long long> all_latencies;
    for (const auto& data : customer_data) {
        all_latencies.insert(all_latencies.end(), data.latencies.begin(), data.latencies.end());
    }

    long long sum = 0, min_lat = LLONG_MAX, max_lat = 0;
    for (auto l : all_latencies) {
        sum += l;
        if (l < min_lat) min_lat = l;
        if (l > max_lat) max_lat = l;
    }
    double avg_latency = (double)sum / all_latencies.size();
    double total_duration_sec = std::chrono::duration_cast<std::chrono::seconds>(end_time - start_time).count();
    double throughput = all_latencies.size() / total_duration_sec;

    // Report stats
    std::cout << avg_latency << "\t" << min_lat << "\t" << max_lat << "\t" << throughput << std::endl;

    return 0;
}
