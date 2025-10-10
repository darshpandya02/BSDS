#include <thread>
#include <vector>
#include <iostream>
#include <chrono>
#include "ServerStub.h"
#include "ExpertQueue.h"

// Expert engineer thread: process special robot requests
void expert_engineer_worker(ExpertQueue& expert_queue, int expert_id) {
    while (true) {
        ExpertRequestPtr req = expert_queue.dequeue();
        std::this_thread::sleep_for(std::chrono::microseconds(100));  // simulate work
        req->robot.expert_id = expert_id;  // attach expert engineer ID
        req->prom.set_value(req->robot);   // notify the waiting engineer thread
    }
}

// Engineer thread: handle each client connection
void engineer_thread(int conn_sock, int engineer_id, ExpertQueue& expert_queue) {
    ServerStub stub;
    stub.Init(conn_sock);

    while (true) {
        Order order;
        int recv_result = recv(conn_sock, (char*)&order, sizeof(order), 0);
        if (recv_result <= 0) break; // client closed or error

        order.deserialize((char*)&order);
        RobotInfo robot;
        robot.customer_id = order.customer_id;
        robot.order_number = order.order_number;
        robot.robot_type = order.robot_type;
        robot.engineer_id = engineer_id;
        robot.expert_id = -1;

        if (order.robot_type == 1) {  // special robot requires expert
            auto req = std::make_shared<ExpertRequest>();
            req->robot = robot;
            expert_queue.enqueue(req);
            std::future<RobotInfo> fut = req->prom.get_future();
            robot = fut.get();  // wait for expert engineer to process
        }

        stub.ShipRobot(robot);
    }
    close(conn_sock);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: ./server port #experts\n";
        return 1;
    }
    int port = std::stoi(argv[1]);
    int num_experts = std::stoi(argv[2]);

    // Setup server socket
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serv_addr{};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);
    bind(listen_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listen_fd, 10);

    ExpertQueue expert_queue;

    // Launch expert engineer thread pool with unique ids starting from 1000
    std::vector<std::thread> expert_threads;
    for (int i = 0; i < num_experts; ++i) {
        expert_threads.emplace_back(expert_engineer_worker, std::ref(expert_queue), 1000 + i);
    }

    int next_engineer_id = 1;
    std::vector<std::thread> engineer_threads;
    std::cout << "Server listening on port " << port << " with " << num_experts << " experts\n";

    while (true) {
        int conn_sock = accept(listen_fd, nullptr, nullptr);
        if (conn_sock < 0) {
            std::cerr << "Accept failed\n";
            continue;
        }
        engineer_threads.emplace_back(engineer_thread, conn_sock, next_engineer_id++, std::ref(expert_queue));
    }

    close(listen_fd);
    for (auto& t : engineer_threads) t.join();
    for (auto& t : expert_threads) t.join();
}
