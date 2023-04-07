#pragma once
#include <map>
#include <vector>
#include "Packet.h"

class Server
{
public:
	explicit Server(int port);
	void Run(float tickSpeed);
	void Bind(int packetId, void (*func)(Packet&));

private:
	void HandleIncomingConnection();
	void HandleReceive();

	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};
	std::map<int, void(*)(Packet&)> m_bindings{};
};