#pragma once
#include <mutex>
#include <vector>

class Server;

class ServerEventManager
{
public:
	enum Event
	{
		Connect,
		Disconnect
	};

	ServerEventManager(int bufferSize = 1);
	~ServerEventManager() = default;
	ServerEventManager(const ServerEventManager& other) = delete;
	ServerEventManager(ServerEventManager&& other) = delete;
	ServerEventManager& operator=(const ServerEventManager& other) = delete;
	ServerEventManager& operator=(ServerEventManager&& other) = delete;

	bool GetEvent(Event& event, int& clientId);

private:
	friend Server;
	bool AddEvent(Event event, int clientId);

	struct ServerEvent
	{
		int clientId{-1};
		Event event{};
	};

	std::mutex m_lockEvent{};
	std::vector<ServerEvent> m_events{};
	int m_eventAmount{};
};