#include "pch.h"
#include <array>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "Packet.h"
#pragma comment(lib, "ws2_32.lib")

int main()
{
    // Initialize Winsock
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsData) != 0) {
        std::cerr << "Error initializing Winsock: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Create a socket for the client
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to the server
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);  // Port number of the server
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);  // IP address of the server

    if (connect(clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    Packet packet{ 500 };
    packet.Write(420.69f);
    packet.Write(69.420f);
    packet.Write(42001);

    while (true)
    {
        if (send(clientSocket, packet.Data(), packet.Length(), 0) == SOCKET_ERROR)
        {
            std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }

        // Receive a response from the server
        std::array<char, 256> buffer{};
        const int bytesReceived = recv(clientSocket, buffer.data(), static_cast<int>(buffer.size()), 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            std::cerr << "Error receiving response: " << WSAGetLastError() << std::endl;
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        
        // Print the response from the server
        std::cout << "Server response: " << buffer.data() << std::endl;

        Sleep(1000);
    }

    //Clean up
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}