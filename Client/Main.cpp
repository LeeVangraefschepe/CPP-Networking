#include "pch.h"
#include <iostream>
#include "Client.h"

void Test(Packet&)
{
    std::cout << "Server sent me something\n";
}

int main()
{
    Packet packet{ 500 };
    packet.Write(420.69f);
    packet.Write(69.420f);
    packet.Write(42001);

    Client client{ 12345, "127.0.0.1" };
    client.Run(20.f);
    client.Bind(777, Test);

    while (true)
    {
        client.SendPacket(packet);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}