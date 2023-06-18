#pragma once
#include <map>
#include <string>
#include <thread>

#include "Packet.h"
#include "PacketManager.h"

class PacketReceiver;

class Client final
{
public:
	explicit Client(int port, const std::string& serverIp, int packetBuffer = 25);
	~Client();

	Client(const Client&) = delete;
	Client(Client&&) = delete;
	Client& operator= (const Client&) = delete;
	Client& operator= (const Client&&) = delete;

	bool GetPacket(Packet& packet) const { return m_packetManager->GetPacket(packet); }
	void Run(float ticks);
	bool IsConnected();
	bool SendPacket(Packet& packet);
	void Bind(PacketReceiver* packetReceiver);

private:
	void InternalRun(float ticks);
	bool HandleReceive();

	std::thread m_clientThread{};
	bool m_connected{false};
	unsigned long long m_socket{};
	PacketReceiver* m_packetReceiver;
	std::unique_ptr<PacketManager> m_packetManager;
};
