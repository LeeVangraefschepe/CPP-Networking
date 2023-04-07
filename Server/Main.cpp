#include "pch.h"
#include <iostream>
#include "Server.h"

void Test(Packet& packet, int)
{
    //Read data from packet
    float health = packet.Read<float>();
    float damage = packet.Read<float>();
    int money = packet.Read<int>();

    //Print data from packet
    std::cout << "health: " << health << "\n";
    std::cout << "damage: " << damage << "\n";
    std::cout << "money: " <<  money << "\n";
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