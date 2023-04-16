
# C++ Networking

This repository contains windows networking without the need of external libraries. It uses the winsock to communicate so a windows machine is required.


# Usage

The data integration in this repository is **packet-based**, with each packet containing a header ID that identifies the intended action to be taken on the server or client side. By using these header IDs, the code is able to differentiate between different types of packets and process them accordingly.

## Create & Run
The constructor of a Server or Client creates a socket by using the windows library.

The Run(float) function will start the listening loop on another thread, so this is **not a blocking call**. The thread will stop working when the destructor is called or if the connection is lost (client side). Don't use the run function multiple times.

Server code:
```cpp
Server server{12345, 10}; //Create server on port 12345 with a max 10 active connection
server.Run(20.f); //Run server @ 20 ticks
```
Client code:
```cpp
Client client{ 12345, "127.0.0.1" }; //Create client on port 12345 & connect to server with ip 127.0.0.1
client.Run(20.f); //Run client @ 20 ticks
```

## Packet
Making a packet requires an `id` for the packet. After that you can write anything to it. Be aware you read it out in the same order as you added it. Reading something will delete the data that it has read.

```cpp
Packet packet{500};
packet.Write(546.5f); //Float value
packet.Write(true); //Bool value
packet.Write(10004); //Int value
packet.WriteString(std::string{"Hello world"}); //String value
```

```cpp
void Packet500Handler(Packet& packet)
{
    //Read data from packet
    const auto health = packet.Read<float>();
    const auto pvp = packet.Read<bool>();
    const auto money = packet.Read<int>();
    std::string message{};
    packet.ReadString(message);

    //Print data from packet
    std::cout << "health: " << health << "\n";
    std::cout << "pvp: " << pvp << "\n";
    std::cout << "money: " << money << "\n";
    std::cout << "message: " << message << "\n";
}
```


## Bind
The `bind` function can be used to link a specific class that derives from either `PacketReceiver` or `ServerEventReceiver` to the Server or Client. By doing so, developers can customize how data is handled and processed based on their specific needs.
### ServerEventReceiver example
```cpp
class ServerHandler : public ServerEventReceiver
{
public:
    void OnConnect(int clientId) const override
    {
        std::cout << "New client connected with id:" << clientId << "\n";
    }
    void OnDisconnect(int clientId) const override
    {
        std::cout << "Client disconnected with id:" << clientId << "\n";
    }
};
```
```cpp
int main()
{
    Server server{12345, 10};
    server.Run(20.f);

    ServerHandler serverHandler{};
    server.Bind(&serverHandler);
}
```
### Packet receiver example
```cpp
class PacketHandler : public PacketReceiver
{
public:
    void OnReceive(Packet& packet) const override
    {
        const int id = packet.ReadHeaderID();
        if (id == 0)
        {
            std::string message{};
            packet.ReadString(message);
            std::cout << "Got message: " << message << "\n";
            return;
        }
        std::cout << "Got undefined package with id " << id << "\n";
    }
};
```
```cpp
int main()
{
    Client client{ 12345, "127.0.0.1"};
    client.Run(20.f);

    PacketHandler packetHandler{};
    client.Bind(&packetHandler);
}
```
or
```cpp
int main()
{
    Server server{12345, 10};
    server.Run(20.f);

    PacketHandler packetHandler{};
    server.Bind(&packetHandler);
}
```
## Send packet
For servers, there are three variations: send to all clients, send to a specific client, or send to all except one client. This flexibility allows developers to optimize their networked applications for different scenarios. The send function is easy to use, requiring only a packet as input, and can be seamlessly integrated into existing codebases.

Server code:
```cpp
Packet packet{ 777 };
server.SendPacketAll(packet); //Send packet to all clients
server.SendPacket(packet, 0); //Send packet to client id 0
server.SendPacketAllExceptOne(packet, 0) //Send packet to all clients except client id 0
```
Client code:
```cpp
Packet packet{ 500 };
client.SendPacket(packet);
```
