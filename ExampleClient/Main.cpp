#include "pch.h"

//Force color codes to work
#include <Windows.h>
void enableColors()
{
    DWORD consoleMode;
    const HANDLE outputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleMode(outputHandle, &consoleMode))
    {
        SetConsoleMode(outputHandle, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
}
#define GREEN   "\033[32m" 
#define YELLOW  "\033[33m"
#define MAGENTA "\033[35m"
#define RESET   "\033[0m"

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
            std::cout << GREEN << message << "\n" << RESET;
        }
        else if (id == 1)
        {
	        const int amountOnline = packet.Read<int>();
            std::cout << MAGENTA << "There are " << amountOnline - 1 << " people also chatting in this room!\n" << RESET;
        }
        //std::cout << "Server sent me a package with id " << id << "\n";
    }
};

int main()
{
    enableColors();
    std::string line;

    std::cout << YELLOW << "Fill in the server ip:\n" << RESET;
    std::getline(std::cin, line);

    Client client{ 1800, line };
    client.Run(20.f);

    PacketHandler packetHandler{};
    client.Bind(&packetHandler);

    std::cout << YELLOW << "Fill in your username:\n" << RESET;
    std::getline(std::cin, line);

    Packet userData{ 0 };
    userData.WriteString(line);
    client.SendPacket(userData);

    std::cout << "\x1B[2J\x1B[H"; //Clear console

    while (client.IsConnected())
    {
        Packet message{ 1 };
        
        std::getline(std::cin, line);

        message.WriteString(line);

        client.SendPacket(message);
    }
}