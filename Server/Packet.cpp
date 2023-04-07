#include "pch.h"
#include "Packet.h"

#include <bitset>
#include <iostream>
#include <numeric>
#include <string>
#include <sstream>

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
