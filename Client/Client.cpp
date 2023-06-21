#include "pch.h"
#include "Client.h"
#include "Packet.h"
#include <array>
#include <chrono>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "EventPool.h"
#pragma comment(lib, "ws2_32.lib")

Client::Client(int port, const std::string& serverIp, int packetBuffer)
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

    m_packetReceiver = std::make_unique<EventPool<Packet>>(packetBuffer);
    m_packetSender = std::make_unique<EventPool<Packet>>(packetBuffer);

    m_sendThread = std::jthread{ &Client::HandleSend, this };
}

Client::~Client()
{
    m_sendThread.request_stop();
    m_sendCondition.notify_one();
    closesocket(m_socket);
    WSACleanup();
}

void Client::Run(float ticks)
{
    m_connected = true;
    m_clientThread = std::jthread { [this, ticks]() { InternalRun(ticks); } };
    m_clientThread.detach();
}

void Client::SendPacket(const Packet& packet)
{
    m_packetSender->Add(packet);
    m_sendCondition.notify_one();
}

void Client::HandleSend()
{
    const std::stop_token& stopToken{ m_sendThread.get_stop_token() };
    Packet data{};

    while (!stopToken.stop_requested())
    {
        std::unique_lock lock {m_mutex};
        m_sendCondition.wait(lock, [&]() { return m_packetSender->Get(data) || stopToken.stop_requested(); });
        lock.unlock();

        if (stopToken.stop_requested()) { break; }

        if (send(m_socket, data.Data(), data.Length(), 0) == SOCKET_ERROR)
        {
#ifdef _DEBUG
            std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
#endif
            closesocket(m_socket);
            WSACleanup();
            m_connected = false;
        }
    }
}

bool Client::IsConnected()
{
    return m_connected;
}

void Client::InternalRun(float ticks)
{
    const float tickTimeMs{ 1000 / ticks };
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

bool Client::HandleReceive()
{
    // Receive a response from the server
    std::array<char, 256> buffer{};
    const int bytesReceived = recv(m_socket, buffer.data(), static_cast<int>(buffer.size()), 0);
    if (bytesReceived == SOCKET_ERROR)
    {
#ifdef _DEBUG
        std::cerr << "Error receiving response: " << WSAGetLastError() << std::endl;
#endif
        closesocket(m_socket);
        WSACleanup();
        m_connected = false;
        return false;
    }
    if (bytesReceived == 0)
    {
#ifdef _DEBUG
        std::cerr << "No data received closing socket..." << std::endl;
#endif
        closesocket(m_socket);
        WSACleanup();
        m_connected = false;
        return false;
    }

    //Create packet core
    std::vector<char> charBuffer{ std::begin(buffer), std::end(buffer) };

    //WARNING PACKET CREATION WILL DELETE CHAR BUFFER
    m_packetReceiver->Add(Packet{charBuffer});

    return true;
}
