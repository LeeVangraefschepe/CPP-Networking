#pragma once
#include <map>
#include <thread>
#include <vector>
#include "Packet.h"

class Server final
{
public:
	explicit Server(int port);
	~Server();

	Server(const Server&) = delete;
	Server(Server&&) = delete;
	Server& operator= (const Server&) = delete;
	Server& operator= (const Server&&) = delete;

	void Run(float tickSpeed);
	bool SendPacket(Packet& packet, int id);
	void SendPacketAll(Packet& packet);
	void Bind(int packetId, void (*func)(Packet&, int));

private:
	void InternalRun(float ticks);

	void HandleIncomingConnection();
	void HandleReceive();

	std::thread m_serverThread{};
	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};
	std::map<int, void(*)(Packet&, int)> m_bindings{};
};