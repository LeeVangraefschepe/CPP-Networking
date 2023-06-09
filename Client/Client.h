#pragma once
#include <map>
#include <string>
#include <thread>

//Includes needed for unique pointer
#include "Packet.h"
#include "EventPool.h"

class Client final
{
public:
	explicit Client(int port, const std::string& serverIp, int packetBuffer = 25);
	~Client();

	Client(const Client&) = delete;
	Client(Client&&) = delete;
	Client& operator= (const Client&) = delete;
	Client& operator= (const Client&&) = delete;

	bool GetPacket(Packet& packet) const { return m_packetReceiver->Get(packet); }
	void SendPacket(const Packet& packet);
	void Run(float ticks);
	void HandleSend();
	bool IsConnected();

private:
	void InternalRun(float ticks);
	bool HandleReceive();

	std::jthread m_clientThread{};

	std::jthread m_sendThread;
	std::condition_variable m_sendCondition{};

	bool m_connected{false};
	unsigned long long m_socket{};
	std::unique_ptr<EventPool<Packet>> m_packetReceiver;
	std::unique_ptr<EventPool<Packet>> m_packetSender;

	std::mutex m_mutex{};
};
