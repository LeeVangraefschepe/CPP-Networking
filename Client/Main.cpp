#include "pch.h"
#include <iostream>
#include "Client.h"
#include "PacketReceiver.h"

class PacketHandler : public PacketReceiver
{
public:
    void OnReceive(Packet& packet) const override
    {
        const int id = packet.ReadHeaderID();
        std::cout << "Server sent me a package with id " << id << "\n";
    }
};

int main()
{
    Packet packet{ 500 };
    packet.Write(420.69f);
    packet.Write(true);
    packet.Write(42001);

    Client client{ 12345, "127.0.0.1" };
    client.Run(20.f);

    PacketHandler packetHandler{};
    client.Bind(&packetHandler);

    while (client.IsConnected())
    {
        client.SendPacket(packet);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}