#include "pch.h"
#include "PacketManager.h"
#include <iostream>

PacketManager::PacketManager(int bufferSize)
{
	std::unique_lock lock {m_lockPackets};
	m_packets.reserve(bufferSize);
	for (int i = 0; i < bufferSize; i++)
	{
		//Temp objects
		Packet packet{-1};
		m_packets.push_back(packet);
	}
}

bool PacketManager::GetPacket(Packet& packet)
{
	std::unique_lock lock {m_lockPackets};
	if (m_packetAmount < 1) {return false;}
	packet = m_packets[--m_packetAmount];
	return true;
}

bool PacketManager::AddPacket(Packet packet)
{
	std::unique_lock lock {m_lockPackets};
	if (m_packetAmount >= static_cast<int>(m_packets.size())-1)
	{
		std::cout << "Package is not saved. Increase buffer size\n";
		return true;
	}

	m_packets[m_packetAmount++] = packet;
	return false;
}
