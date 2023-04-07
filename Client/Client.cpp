#include "pch.h"
#include "Client.h"
#include "Packet.h"
#include <array>
#include <chrono>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

Client::Client(int port, const std::string& serverIp)
{
    //Initialize Winsock
    WSADATA wsData;
    WORD version = MAKEWORD(2, 2);
    if (WSAStartup(version, &wsData) != 0)
    {
        std::cerr << "Error initializing Winsock: " << WSAGetLastError() << std::endl;
        return;
    }

    //Create a socket for the client
    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == INVALID_SOCKET)
    {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    //Connect to the server
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(static_cast<u_short>(port));  //Port number of the server
    inet_pton(AF_INET, serverIp.c_str(), &serverAddress.sin_addr);  //IP address of the server

    if (connect(m_socket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR)
    {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return;
    }
}

Client::~Client()
{
    m_clientThread.join();
    closesocket(m_socket);
    WSACleanup();
}

void Client::Run(float ticks)
{
    m_clientThread = std::thread { [this, ticks]() { InternalRun(ticks); } };
    m_clientThread.detach();
}
void Client::InternalRun(float ticks)
{
    const float tickTimeMs{ 1000 / ticks };
    std::cout << "Running at " << ticks << "ticks per second\n";
    auto end = std::chrono::high_resolution_clock::now();
    bool connected{true};
    while (connected)
    {
        const auto currentTime = std::chrono::high_resolution_clock::now();
        end = currentTime;

        connected = HandleReceive();

        const auto sleepTimeMs = tickTimeMs - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - currentTime).count();
        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(sleepTimeMs)));
    }
}

void Client::SendPacket(Packet& packet)
{
    if (send(m_socket, packet.Data(), packet.Length(), 0) == SOCKET_ERROR)
    {
        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
    }
}

void Client::Bind(int packetId, void(* func)(Packet&))
{
    if (m_bindings.contains(packetId))
    {
        std::cerr << "WARNING: Binding already exists and has been overwritten (" << packetId << ")\n";
    }
    std::pair key{ packetId, func };
    m_bindings.emplace(key);
}

bool Client::HandleReceive()
{
    // Receive a response from the server
    std::array<char, 256> buffer{};
    const int bytesReceived = recv(m_socket, buffer.data(), static_cast<int>(buffer.size()), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
        std::cerr << "Error receiving response: " << WSAGetLastError() << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return false;
    }
    if (bytesReceived == 0)
    {
        std::cerr << "No data received closing socket..." << std::endl;
        closesocket(m_socket);
        WSACleanup();
        return false;
    }

    //Create packet core
    std::vector<char> charBuffer{ std::begin(buffer), std::end(buffer) };
    Packet packet{ charBuffer };
    const int header = packet.ReadHeaderID();

    //Print the response from the server
    std::cout << "Received " << bytesReceived << " bytes from client with id: " << header << std::endl;

    //WARNING PACKET CREATION WILL DELETE BUFFER
    for (const auto& packetId : m_bindings)
    {
        if (packetId.first == header)
        {
            packetId.second(packet);
            break;
        }
    }
    return true;
}
