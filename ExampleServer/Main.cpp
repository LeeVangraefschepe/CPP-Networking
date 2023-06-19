#include "pch.h"
#include <Server.h>
#include "UserManager.h"

UserManager gUserManager{};
Server gServer{ 12345,10 };

int main()
{
    gServer.Run(20.f);
    const auto eventHandler = gServer.GetEventManager();

    //Alloc for future usage
    int clientId{-1};
    Packet packet{-1};
    ServerEventManager::Event event{};

    while (true)
    {
        while (eventHandler->GetEvent(event, clientId))
        {
	        switch (event)
	        {
	        case ServerEventManager::Connect:
                gUserManager.AddUser(clientId, User{});
                break;
	        case ServerEventManager::Disconnect:
                gUserManager.RemoveUser(clientId);
                break;
	        }
        }

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