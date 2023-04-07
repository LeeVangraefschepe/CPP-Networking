#pragma once
#include <map>
#include <string>
#include <thread>

#include "Packet.h"

class Client final
{
public:
	explicit Client(int port, const std::string& serverIp);
	~Client();
	void Run(float ticks);
	void SendPacket(Packet& packet);
private:
	void InternalRun(float ticks);
	void HandleReceive();

	std::thread m_clientThread{};
	unsigned long long m_socket{};
	std::map<int, void(*)(Packet&)> m_bindings{};
};
