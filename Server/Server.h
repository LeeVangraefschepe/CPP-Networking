#pragma once
#include <map>
#include <thread>
#include <vector>
#include "Packet.h"

class PacketReceiver;
class ServerEventReceiver;

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
	void SendPacketAllExceptOne(Packet& packet, int id);
	void SendPacketAll(Packet& packet);
	void Bind(PacketReceiver* packetReceiver);
	void Bind(ServerEventReceiver* receiver);
	void UnBind(ServerEventReceiver* receiver);

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
	PacketReceiver* m_packetReceiver;
	std::vector<ServerEventReceiver*> m_receivers;
};