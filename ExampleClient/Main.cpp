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

int main()
{
    enableColors();
    std::string line;

    std::cout << YELLOW << "Fill in the server ip:\n" << RESET;
    std::getline(std::cin, line);
    if (line.empty()) { line = "127.0.0.1"; }

    Client client{ 12345, line };
    client.Run(20.f);

    std::cout << YELLOW << "Fill in your username:\n" << RESET;
    std::getline(std::cin, line);
    if (line.empty()) { line = "Anonymous"; }

    Packet userData{ 0 };
    userData.WriteString(line);
    client.SendPacket(userData);

    std::cout << "\x1B[2J\x1B[H"; //Clear console

    //Create lamda to handle incoming messages on other thread
    auto packetHandle = [&]()
    {
        Packet packet{ -1 };
        while (true)
        {
            if (client.GetPacket(packet))
            {
	            if (const int id = packet.ReadHeaderID(); id == 0)
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
            }
	    }
    };

    //Detach other thread
    std::jthread thread{packetHandle};
    thread.detach();

    //Start looping for input
    while (client.IsConnected())
    {
        Packet message{ 1 };
        
        std::getline(std::cin, line);

        message.WriteString(line);

        client.SendPacket(message);
    }
}