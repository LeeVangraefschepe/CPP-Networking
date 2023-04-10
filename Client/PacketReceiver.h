#pragma once
#include "Packet.h"
class PacketReceiver
{
public:
	virtual ~PacketReceiver() = default;
	virtual void OnReceive(Packet& packet) const = 0;
};