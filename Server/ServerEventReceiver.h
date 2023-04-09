#pragma once
class ServerEventReceiver
{
public:
	virtual ~ServerEventReceiver() = default;
	virtual void OnDisconnect(int clientId) const = 0;
	virtual void OnConnect(int clientId) const = 0;
};