#include "pch.h"
#include <iostream>
#include <Client.h>
#include <PacketReceiver.h>

class PacketHandler : public PacketReceiver
{
public:
    void OnReceive(Packet& packet) const override
    {
        const int id = packet.ReadHeaderID();
        if (id == 0)
        {
            std::string message{};
            packet.ReadString(message);
            std::cout << message << "\n";
        }
        else if (id == 1)
        {
	        const int amountOnline = packet.Read<int>();
            std::cout << "There are " << amountOnline - 1 << " people also chatting in this room!\n";
        }
        //std::cout << "Server sent me a package with id " << id << "\n";
    }
};

int main()
{
    std::string line;

    std::cout << "Fill in the server ip:\n";
    std::getline(std::cin, line);

    Client client{ 12345, line };
    client.Run(20.f);

    PacketHandler packetHandler{};
    client.Bind(&packetHandler);

    std::cout << "Fill in your username:\n";
    std::getline(std::cin, line);

    Packet userData{ 0 };
    userData.WriteString(line);
    client.SendPacket(userData);

    while (client.IsConnected())
    {
        Packet message{ 1 };
        
        std::getline(std::cin, line);

        message.WriteString(line);

        client.SendPacket(message);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}