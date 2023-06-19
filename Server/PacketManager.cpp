#include "pch.h"
#include "PacketManager.h"
#include <iostream>
#include <utility>

PacketManager::PacketManager(int bufferSize)
{
	std::unique_lock lock {m_lockPackets};
	m_packets.reserve(bufferSize);
	for (int i = 0; i < bufferSize; i++)
	{
		//Temp objects
		m_packets.emplace_back(PacketData{-1, Packet { -1 }});
	}
}

bool PacketManager::GetPacket(Packet& packet, int& clientId)
{
	std::unique_lock lock {m_lockPackets};
	if (m_packetAmount < 1) { return false; }
	const auto& packetData = m_packets[--m_packetAmount];
	clientId = packetData.clientId;
	packet = packetData.packet;
	return true;
}

bool PacketManager::AddPacket(Packet packet, int clientId)
{
	std::unique_lock lock {m_lockPackets};
	if (m_packetAmount >= static_cast<int>(m_packets.size()) - 1)
	{
		std::cout << "Package is not saved. Increase buffer size\n";
		return true;
	}

	m_packets[m_packetAmount++] = PacketData{clientId, std::move(packet)};
	return false;
}