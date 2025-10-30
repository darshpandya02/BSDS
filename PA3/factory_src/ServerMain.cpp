// #include <chrono>
// #include <iostream>
// #include <mutex>
// #include <thread>
// #include <vector>

// #include "ServerSocket.h"
// #include "ServerThread.h"

// int main(int argc, char *argv[]) {
// 	int port;
// 	int engineer_cnt = 0;
// 	int num_experts;
// 	ServerSocket socket;
// 	RobotFactory factory;
// 	std::unique_ptr<ServerSocket> new_socket;
// 	std::vector<std::thread> thread_vector;
	
// 	if (argc < 3) {
// 		std::cout << "not enough arguments" << std::endl;
// 		std::cout << argv[0] << "[port #] [# experts]" << std::endl;
// 		return 0;
// 	}
// 	port = atoi(argv[1]);
// 	num_experts = atoi(argv[2]);

// 	for (int i = 0; i < num_experts; i++) {
// 		std::thread expert_thread(&RobotFactory::ExpertThread, &factory, engineer_cnt++);
// 		thread_vector.push_back(std::move(expert_thread));
// 	}

// 	if (!socket.Init(port)) {
// 		std::cout << "Socket initialization failed" << std::endl;
// 		return 0;
// 	}

// 	while ((new_socket = socket.Accept())) {
// 		std::thread engineer_thread(&RobotFactory::EngineerThread, &factory, 
// 				std::move(new_socket), engineer_cnt++);
// 		thread_vector.push_back(std::move(engineer_thread));
// 	}
// 	return 0;
// }

#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "ServerSocket.h"
#include "ServerThread.h"

int main(int argc, char *argv[]) {
    int port;
    int engineer_cnt = 0;
    int num_admins;
    ServerSocket socket;
    RobotFactory factory;
    std::unique_ptr<ServerSocket> new_socket;
    std::vector<std::thread> thread_vector;
    
    if (argc < 3) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << " [port #] [# admins]" << std::endl;
        return 0;
    }
    port = atoi(argv[1]);
    num_admins = atoi(argv[2]);

    // Create admin threads (must be exactly 1 per requirement)
    if (num_admins != 1) {
        std::cout << "Number of admins must be 1" << std::endl;
        return 0;
    }

    for (int i = 0; i < num_admins; i++) {
        std::thread admin_thread(&RobotFactory::AdminThread, &factory, i);
        thread_vector.push_back(std::move(admin_thread));
    }

    if (!socket.Init(port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }

    // Accept engineer connections
    while ((new_socket = socket.Accept())) {
        std::thread engineer_thread(&RobotFactory::EngineerThread, &factory, 
                std::move(new_socket), engineer_cnt++);
        thread_vector.push_back(std::move(engineer_thread));
    }
    
    return 0;
}