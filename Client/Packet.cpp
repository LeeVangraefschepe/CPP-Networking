#include "pch.h"
#include "Packet.h"

int Packet::ReadHeaderID()
{
	return Read<int>();
}

void Packet::ReadString(std::string& string)
{
    //Find size
    const auto size = Read<unsigned int>();

    //Set iterator ready
    auto it = m_data.begin();
    advance(it, size);

    //Create ss with binary value
    std::stringstream ss(accumulate(m_data.begin(), it, std::string("")), std::ios_base::in | std::ios_base::binary);

    //Read value
    std::vector<char> buffer(size);
    ss.read(buffer.data(), size);

    //Return string
    string = { buffer.data(), size };
}

void Packet::WriteString(const std::string& string)
{
    //Get size & write it
    const auto size = static_cast<unsigned>(string.size());
    Write<unsigned int>(size);

    //Write string data binary
    std::stringstream ss{ std::ios::out | std::ios::binary };
    ss.write(string.c_str(), size);

    //Save binary string data to m_data
    const auto data{ ss.str() };
    for (char byte : data)
    {
        m_data.push_back(byte);
    }
}

char* Packet::Data()
{
    return &*m_data.begin();
}

int Packet::Length() const
{
	return static_cast<int>(m_data.size());
}