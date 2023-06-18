#pragma once
#include <mutex>
#include <vector>
#include "Packet.h"

class Client;

class PacketManager final
{
public:
	PacketManager(int bufferSize = 1);
	~PacketManager() = default;
	PacketManager(const PacketManager& other) = delete;
	PacketManager(PacketManager&& other) = delete;
	PacketManager& operator=(const PacketManager& other) = delete;
	PacketManager& operator=(PacketManager&& other) = delete;

	bool GetPacket(Packet& packet);

private:
	friend Client;
	/**
	 * \return True means that the buffer is full and the package is not added
	 */
	bool AddPacket(Packet packet);

	std::mutex m_lockPackets{};
	std::vector<Packet> m_packets{};
	int m_packetAmount{};
};

