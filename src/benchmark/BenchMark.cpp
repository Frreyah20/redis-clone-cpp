#include <iostream>
#include <string>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>
#include <unistd.h>


void runClient(int requests)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(6379);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);
    connect(sock, (sockaddr*)&server, sizeof(server));
    for(int i = 0; i < requests; i++)
    {
        std::string cmd = "PING\n";
        send(sock, cmd.c_str(), cmd.size(), 0);
        char buffer[1024];
        recv(sock, buffer, sizeof(buffer), 0);
    }
    close(sock);
}

int main()
{
    int CLIENTS;
    std::cout << "Clients: ";
    std::cin >> CLIENTS;
    const int REQUESTS_PER_CLIENT = 5000;
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    for(int i = 0; i < CLIENTS; i++)
    {
        threads.emplace_back(runClient, REQUESTS_PER_CLIENT);
    }

    for(auto& t : threads)
    {
        t.join();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>( end - start);
    int total_requests = CLIENTS * REQUESTS_PER_CLIENT;
    double throughput = total_requests / (duration.count() / 1000.0);
    std::cout << "Clients: " << CLIENTS << std::endl;
    std::cout << "Requests: " << total_requests << std::endl;
    std::cout << "Time(ms): " << duration.count() << std::endl;
    std::cout << "Throughput: " << throughput << " req/s" << std::endl;
    return 0;
}