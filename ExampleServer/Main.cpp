#include "pch.h"
#include <iostream>
#include <Server.h>
#include <PacketReceiver.h>
#include <ServerEventReceiver.h>

class PacketHandler : public PacketReceiver
{
public:
    void OnReceive(int clientId, Packet& packet) const override
    {
        const int id = packet.ReadHeaderID();
        if (id == 500)
        {
            //Read data from packet
            const auto health = packet.Read<float>();
            const auto pvp = packet.Read<bool>();
            const auto money = packet.Read<int>();

            //Print data from packet
            std::cout << "health: " << health << "\n";
            std::cout << "damage: " << pvp << "\n";
            std::cout << "money: " << money << "\n";
        }
    }
};

class ServerHandler : public ServerEventReceiver
{
public:
    void OnConnect(int clientId) const override
    {
        std::cout << "Client connected on id " << clientId << "\n";
    }
    void OnDisconnect(int clientId) const override
    {
        std::cout << "Client disconnected on id " << clientId << "\n";
    }
};

int main()
{
    Server server{ 12345,10 };
    server.Run(20.f);

    PacketHandler packetHandler{};
    server.Bind(&packetHandler);

    ServerHandler serverHandler{};
    server.Bind(&serverHandler);

    Packet packet{ 777 };
    while (true)
    {
        server.SendPacketAll(packet);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}