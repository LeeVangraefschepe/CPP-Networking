#pragma once
#include <numeric>
#include <sstream>
#include <utility>
#include <vector>

class Packet
{
public:
	Packet(int headerId = -1) { Write(headerId); }
	explicit Packet(std::vector<char>& data) : m_data(std::move(data)) {}

    char* Data();
    int Length() const;

	int ReadHeaderID();

	template <typename T>
	T Read()
	{
        //Move the iterator to the size position
        auto it = m_data.begin();
        advance(it, sizeof(T));

        //Create ss with binary value
        std::stringstream ss(accumulate(m_data.begin(), it, std::string("")), std::ios_base::in | std::ios_base::binary);

        //Read value
        T value{};
        ss.read(reinterpret_cast<char*>(&value), sizeof(T));

        //Delete readed value
        m_data.erase(m_data.begin(), it);

        return value;
	}
    void ReadString(std::string& string);
	
	

	template <typename T>
	void Write(T value)
    {
        std::stringstream ss{ std::ios::out | std::ios::binary };
		ss.write(reinterpret_cast<char*>(&value), sizeof(value));
        const auto data{ ss.str() };
        for (char byte : data)
        {
            m_data.push_back(byte);
        }
    }
    void WriteString(const std::string& string);

private:
	std::vector<char> m_data{};
};
