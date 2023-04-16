#pragma once
class ServerEventReceiver
{
public:
	virtual ~ServerEventReceiver() = default;
	virtual void OnDisconnect(int clientId) = 0;
	virtual void OnConnect(int clientId) = 0;
};