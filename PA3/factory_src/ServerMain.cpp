// #include <chrono>
// #include <iostream>
// #include <mutex>
// #include <thread>
// #include <vector>

// #include "ServerSocket.h"
// #include "ServerThread.h"

// int main(int argc, char *argv[]) {
//     int port;
//     int engineer_cnt = 0;
//     int num_admins;
//     ServerSocket socket;
//     RobotFactory factory;
//     std::unique_ptr<ServerSocket> new_socket;
//     std::vector<std::thread> thread_vector;
    
//     if (argc < 3) {
//         std::cout << "not enough arguments" << std::endl;
//         std::cout << argv[0] << " [port #] [# admins]" << std::endl;
//         return 0;
//     }
//     port = atoi(argv[1]);
//     num_admins = atoi(argv[2]);

//     // Create admin threads (must be exactly 1 per requirement)
//     if (num_admins != 1) {
//         std::cout << "Number of admins must be 1" << std::endl;
//         return 0;
//     }

//     for (int i = 0; i < num_admins; i++) {
//         std::thread admin_thread(&RobotFactory::AdminThread, &factory, i);
//         thread_vector.push_back(std::move(admin_thread));
//     }

//     if (!socket.Init(port)) {
//         std::cout << "Socket initialization failed" << std::endl;
//         return 0;
//     }

//     // Accept engineer connections
//     while ((new_socket = socket.Accept())) {
//         std::thread engineer_thread(&RobotFactory::EngineerThread, &factory, 
//                 std::move(new_socket), engineer_cnt++);
//         thread_vector.push_back(std::move(engineer_thread));
//     }
    
//     return 0;
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
    int unique_id;
    int num_peers;
    std::vector<PeerInfo> peers;
    
    if (argc < 4) {
        std::cout << "not enough arguments" << std::endl;
        std::cout << argv[0] << " [port #] [unique ID] [# peers] ";
        std::cout << "(repeat [ID] [IP] [port #])" << std::endl;
        return 0;
    }
    
    port = atoi(argv[1]);
    unique_id = atoi(argv[2]);
    num_peers = atoi(argv[3]);

    // Parse peer information
    if (argc < 4 + num_peers * 3) {
        std::cout << "not enough peer information" << std::endl;
        return 0;
    }

    for (int i = 0; i < num_peers; i++) {
        PeerInfo peer;
        peer.id = atoi(argv[4 + i * 3]);
        peer.ip = argv[4 + i * 3 + 1];
        peer.port = atoi(argv[4 + i * 3 + 2]);
        peers.push_back(peer);
    }

    ServerSocket socket;
    RobotFactory factory(unique_id, peers);
    std::unique_ptr<ServerSocket> new_socket;
    std::vector<std::thread> thread_vector;

    // Start admin thread (PFA)
    std::thread admin_thread(&RobotFactory::AdminThread, &factory, 0);
    thread_vector.push_back(std::move(admin_thread));

    if (!socket.Init(port)) {
        std::cout << "Socket initialization failed" << std::endl;
        return 0;
    }

    int engineer_cnt = 0;
    // Accept connections (could be customers or PFAs)
    while ((new_socket = socket.Accept())) {
        std::thread engineer_thread(&RobotFactory::EngineerThread, &factory, 
                std::move(new_socket), engineer_cnt++);
        thread_vector.push_back(std::move(engineer_thread));
    }
    
    return 0;
}