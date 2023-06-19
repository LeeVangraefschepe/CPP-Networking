#pragma once
#include <map>
#include <thread>
#include <vector>
#include "Packet.h"
#include "PacketManager.h"
#include "ServerEventManager.h"

class PacketReceiver;
class ServerEventReceiver;

class Server final
{
public:
	explicit Server(int port, int maxClients, int packetBuffer = 50, int eventBuffer = 10);
	~Server();

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	Server& operator= (const Server&) = delete;
	Server& operator= (const Server&&) = delete;

	void Run(float tickSpeed);
	bool SendPacket(Packet& packet, int id);
	void SendPacketAllExceptOne(Packet& packet, int id);
	void SendPacketAll(Packet& packet);

	bool GetPacket(Packet& packet, int& clientId) const { return m_packetManager->GetPacket(packet, clientId); }
	ServerEventManager* GetEventManager() const { return m_eventManager.get(); }

	int GetConnectedAmount() const;
	int GetMaxClients() const;

private:
	void InternalRun(float ticks);

	void HandleIncomingConnection();
	void HandleReceive();

	bool FindAvailableSlot(int& id) const;

	void Connect(int id) const;
	void Disconnect(int id);

	std::thread m_serverThread{};
	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};

	std::unique_ptr<PacketManager> m_packetManager;
	std::unique_ptr<ServerEventManager> m_eventManager;
};