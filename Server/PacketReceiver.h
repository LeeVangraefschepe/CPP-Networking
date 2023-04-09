#pragma once
#include "Packet.h"
class PacketReceiver
{
public:
	virtual ~PacketReceiver() = default;
	virtual void OnReceive(int clientId, Packet& packet) const = 0;
};