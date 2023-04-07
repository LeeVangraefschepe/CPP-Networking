#pragma once
#include <vector>

class Server
{
public:
	explicit Server(int port);
	void Run();
private:
	void HandleIncomingConnection();
	void HandleReceive();
	unsigned long long m_socket;
	std::vector<unsigned long long> m_clients{};
};
