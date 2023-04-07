#include "pch.h"
/*#include <array>
#include <iostream>
#include <winsock2.h>//For Windows socket programming
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")//Link with the Winsock library*/
#include "Server.h"

int main() {
    // Initialize Winsock

    Server server{12345};
    server.Run();

    return 0;
    /*WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
        return 1;
    }

    // Create the socket for the server
    const SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating server socket: " << WSAGetLastError() << std::endl;
        WSACleanup();  // Clean up Winsock
        return 1;
    }

    // Bind the server socket to a specific IP address and port
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;  // Bind to any available IP address
    serverAddress.sin_port = htons(12345);  // Use port 12345

    result = bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress));
    if (result == SOCKET_ERROR) {
        std::cerr << "Error binding server socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    result = listen(serverSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error listening on server socket: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server is listening on port 12345..." << std::endl;

    // Accept incoming connections from clients and start communicating with them
    sockaddr_in clientAddress{};
    int clientAddressSize = sizeof(clientAddress);
    const SOCKET clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddress), &clientAddressSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error accepting connection: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::array<char, INET_ADDRSTRLEN> ipAddressBuffer{};
    inet_ntop(AF_INET, &clientAddress.sin_addr, ipAddressBuffer.data(), ipAddressBuffer.size());
    std::cout << "Client connected from " << ipAddressBuffer.data() << std::endl;


    char buffer[1024];
    const int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead == SOCKET_ERROR) {
        std::cerr << "Error receiving data: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    buffer[bytesRead] = '\0';  // Null-terminate the received data
    std::cout << "Received data: " << buffer << std::endl;

    const std::string responseData{"Hello from the server!"};
    const char* response = responseData.c_str();
    const int bytesSent = send(clientSocket, response, static_cast<int>(strlen(response)), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error sending data: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Sent " << bytesSent << " bytes to client." << std::endl;

    // Close the sockets and clean up Winsock
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;*/
}