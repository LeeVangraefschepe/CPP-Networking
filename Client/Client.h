#pragma once
#include <map>
#include <string>
#include <thread>

//Includes needed for unique pointer
#include "Packet.h"
#include "EventPool.h"

struct sockaddr_in;

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
	void SendTCPPacket(const Packet& packet);
	void SendUDPPacket(const Packet& packet);
	void Run(float ticks);
	bool IsConnected() const;

private:
	struct InternalPacket final
	{
		Packet Packet{};
		bool IsUDP{};
	};

	void InternalRun(float ticks);
	bool HandleReceive();
	void HandleSend();

	std::jthread m_clientThread{};

	std::jthread m_sendThread;
	std::condition_variable m_sendCondition{};

	bool m_connected{false};
	unsigned long long m_TCPsocket{};
	unsigned long long m_UDPsocket{};
	std::unique_ptr<sockaddr_in> m_pServerAdress;

	std::unique_ptr<EventPool<Packet>> m_packetReceiver;
	std::unique_ptr<EventPool<InternalPacket>> m_packetSender;

	std::mutex m_mutex{};
};
