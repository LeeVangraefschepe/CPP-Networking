#include "pch.h"
#include "Packet.h"

int Packet::ReadHeaderID()
{
	return Read<int>();
}

char* Packet::Data()
{
    return &*m_data.begin();
}

int Packet::Length() const
{
	return static_cast<int>(m_data.size());
}