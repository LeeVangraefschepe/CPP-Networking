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
	bool SendPacket(Packet& packet);
	void Bind(int packetId, void (*func)(Packet&));

private:
	void InternalRun(float ticks);
	bool HandleReceive();

	std::thread m_clientThread{};
	unsigned long long m_socket{};
	std::map<int, void(*)(Packet&)> m_bindings{};
};
