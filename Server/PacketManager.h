#pragma once
#include <mutex>
#include <vector>
#include "Packet.h"

class Server;

class PacketManager final
{
public:
	PacketManager(int bufferSize = 1);
	~PacketManager() = default;
	PacketManager(const PacketManager& other) = delete;
	PacketManager(PacketManager&& other) = delete;
	PacketManager& operator=(const PacketManager& other) = delete;
	PacketManager& operator=(PacketManager&& other) = delete;

	bool GetPacket(Packet& packet, int& clientId);

private:
	friend Server;
	/**
	 * \return True means that the buffer is full and the package is not added
	 */
	bool AddPacket(Packet packet, int clientId);

	struct PacketData
	{
		int clientId;
		Packet packet;
	};

	std::mutex m_lockPackets{};
	std::vector<PacketData> m_packets{};
	int m_packetAmount{};
};