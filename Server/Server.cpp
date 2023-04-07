#include "pch.h"
#include "Server.h"
#include <array>
#include <iostream>
#include <thread>
#include <winsock2.h>//For Windows socket programming
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")//Link with the Winsock library

Server::Server(int port)
{
    //Initialize Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return;
    }

    //Create the socket for the server (TCP)
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        std::cerr << "Error creating server socket: " << WSAGetLastError() << std::endl;

    	//Clean up Winsock
        WSACleanup();

        return;
    }

    //Create socket info for binding
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Bind to any available IP address
    serverAddress.sin_port = htons(static_cast<u_short>(port));  // Use port 12345

    //Bind the server socket to a specific IP address and port
    result = bind(m_socket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Error binding server socket: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return;
    }

    //Listen for incoming connections
    result = listen(m_socket, SOMAXCONN);
    if (result == SOCKET_ERROR)
    {
        std::cerr << "Error listening on server socket: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return;
    }

    std::cout << "Server is listening on port 12345..." << std::endl;
}

void Server::Run(float ticks)
{
    const float tickTimeMs{ 1000 / ticks };
    std::cout << "Running at " << ticks << "ticks per second\n";
    auto end = std::chrono::high_resolution_clock::now();
	while (true)
	{
        const auto currentTime = std::chrono::high_resolution_clock::now();
        end = currentTime;

        HandleIncomingConnection();
        HandleReceive();

        const auto sleepTimeMs = tickTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
	}
}

void Server::HandleIncomingConnection()
{
    // Set up a set of sockets to monitor for incoming connections
    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(m_socket, &readSet);

    // Set a timeout for select() to return if there are no incoming connections
    timeval timeout{};
    timeout.tv_sec = 0;
    timeout.tv_usec = 100; // 1 millisecond

    //Wait for incoming connections
    const int selectResult = select(0, &readSet, nullptr, nullptr, &timeout);
    if (selectResult == SOCKET_ERROR)
    {
        std::cerr << "Error in select(): " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return;
    }
    if (selectResult == 0)
    {
        //No incoming connections, just return
        return;
    }

    //Accept incoming connections from clients and start communicating with them
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);
    const SOCKET clientSocket = accept(m_socket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientSocket == INVALID_SOCKET)
    {
        std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return;
    }
    m_clients.push_back(clientSocket);

    std::array<char, INET_ADDRSTRLEN> ipAddressBuffer{};
    inet_ntop(AF_INET, &clientAddress.sin_addr, ipAddressBuffer.data(), ipAddressBuffer.size());
    std::cout << "Client connected from " << ipAddressBuffer.data() << std::endl;
}


void Server::HandleReceive()
{
    for (auto it = m_clients.begin(); it != m_clients.end(); ++it)
    {
        const SOCKET& clientSocket = *it;
        // Receive data from the client
        std::array<char,256> buffer{};
        const int bytesReceived = recv(clientSocket, buffer.data(), static_cast<int>(buffer.size()), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            m_clients.erase(it);
            return;
        }
        if (bytesReceived == 0)
        {
            m_clients.erase(it);
            return;
        }

        // Print the received data
        std::cout << "Received " << bytesReceived << " bytes from client: " << buffer.data() << std::endl;

        //WARNING PACKET CREATION WILL DELETE BUFFER
        std::vector<char> charBuffer{ std::begin(buffer), std::end(buffer) };
        Packet packet{ charBuffer };
        auto header = packet.ReadHeaderID();
        auto health = packet.Read<float>();
        auto damage = packet.Read<float>();
        auto money = packet.Read<int>();

        std::cout << "header: " << header << "\n";
        std::cout << "health: " << health << "\n";
        std::cout << "damage: " << damage << "\n";
        std::cout << "money: " << money << "\n";

        int stop{};

        // Echo the data back to the client
        if (send(clientSocket, buffer.data(), bytesReceived, 0) == SOCKET_ERROR)
        {
            std::cerr << "Error sending data: " << WSAGetLastError() << std::endl;
        }
    }
}