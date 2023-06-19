#include "pch.h"
#include "ServerEventManager.h"

#include <iostream>

ServerEventManager::ServerEventManager(int bufferSize)
{
	m_events.resize(bufferSize);
}

bool ServerEventManager::GetEvent(Event& event, int& clientId)
{
	std::unique_lock lock {m_lockEvent};
	if (m_eventAmount < 1) { return false; }
	const auto& packetData = m_events[--m_eventAmount];
	clientId = packetData.clientId;
	event = packetData.event;
	return true;
}

bool ServerEventManager::AddEvent(Event event, int clientId)
{
	std::unique_lock lock {m_lockEvent};
	if (m_eventAmount >= static_cast<int>(m_events.size()) - 1)
	{
		std::cout << "Package is not saved. Increase buffer size\n";
		return true;
	}

	m_events[m_eventAmount++] = ServerEvent{clientId, event};
	return false;
}