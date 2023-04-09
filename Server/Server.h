#pragma once
#include <map>
#include <thread>
#include <vector>
#include "Packet.h"

class PacketReceiver;

class Server final
{
public:
	explicit Server(int port, int maxClients);
	~Server();

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	Server& operator= (const Server&) = delete;
	Server& operator= (const Server&&) = delete;

	void Run(float tickSpeed);
	bool SendPacket(Packet& packet, int id);
	void SendPacketAll(Packet& packet);
	void Bind(PacketReceiver* packetReceiver);

private:
	void InternalRun(float ticks);

	void HandleIncomingConnection();
	void HandleReceive();

	bool FindAvailableSlot(int& id) const;
	int GetConnectedAmount() const;
	int GetMaxClients() const;

	std::thread m_serverThread{};
	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};
	std::vector<PacketReceiver*> m_receivers;
};