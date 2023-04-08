#include "pch.h"
#include <iostream>
#include "Server.h"

void Test(Packet& packet, int)
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

int main()
{
    Server server{12345};
    server.Bind(500, Test);
    server.Run(20.f);

    Packet packet{ 777 };

    while (true)
    {
        server.SendPacketAll(packet);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    return 0;
}