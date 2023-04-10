#include "pch.h"
#include <iostream>
#include <Server.h>
#include <PacketReceiver.h>
#include <ServerEventReceiver.h>

#include "UserManager.h"

UserManager gUserManager{};
Server gServer{ 12345,10 };

class PacketHandler : public PacketReceiver
{
public:
    void OnReceive(int clientId, Packet& packet) const override
    {
        const int id = packet.ReadHeaderID();
        const auto user = gUserManager.GetUser(clientId);
        if (!user) {return;}
        switch (id)
        {
        case 0:
	        {
                std::string username{};
                packet.ReadString(username);
                user->username = username;

                Packet confirm{1};
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
};

class ServerHandler : public ServerEventReceiver
{
public:
    void OnConnect(int clientId) const override
    {
        gUserManager.AddUser(clientId, User{});
    }
    void OnDisconnect(int clientId) const override
    {
        gUserManager.RemoveUser(clientId);
    }
};

int main()
{
    gServer.Run(20.f);

    PacketHandler packetHandler{};
    gServer.Bind(&packetHandler);

    ServerHandler serverHandler{};
    gServer.Bind(&serverHandler);

    while (true)
    {

    }
}