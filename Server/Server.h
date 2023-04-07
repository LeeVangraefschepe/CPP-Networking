#pragma once
#include <vector>
#include "Packet.h"

class Server
{
public:
	explicit Server(int port);
	void Run(float tickSpeed);
private:
	void HandleIncomingConnection();
	void HandleReceive();
	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};
};