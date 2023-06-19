#include "pch.h"
#include <iostream>
#include <Server.h>
#include <ServerEventReceiver.h>

#include "UserManager.h"

UserManager gUserManager{};
Server gServer{ 12345,10 };

class ServerHandler : public ServerEventReceiver
{
public:
    void OnConnect(int clientId) override
    {
        gUserManager.AddUser(clientId, User{});
    }
    void OnDisconnect(int clientId) override
    {
        gUserManager.RemoveUser(clientId);
    }
};

int main()
{
    gServer.Run(20.f);

    ServerHandler serverHandler{};
    gServer.Bind(&serverHandler);

    Packet packet{-1};
    int clientId{-1};

    while (true)
    {
	    while (gServer.GetPacket(packet, clientId))
	    {
            const int id = packet.ReadHeaderID();
            const auto user = gUserManager.GetUser(clientId);
            if (!user) { continue; }

            switch (id)
            {
            case 0:
            {
                std::string username{};
                packet.ReadString(username);
                user->username = username;

                Packet confirm{ 1 };
                confirm.Write<int>(gUserManager.GetUserAmount());
                gServer.SendPacket(confirm, clientId);
            }
            break;
            case 1:
            {
                Packet message{ 0 };
                std::string text{};
                packet.ReadString(text);

                std::stringstream ss{};
                ss << user->username << ": " << text;

                const std::string serverResult = ss.str();

                message.WriteString(serverResult);

                gServer.SendPacketAllExceptOne(message, clientId);
            }
            break;
            default:
                break;
            }
	    }
    }
}