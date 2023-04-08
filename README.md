# C++ Networking

This repository contains windows networking without the need of external libraries. It uses the winsock to communicate so a windows machine is required.


# Usage

The data integration works with **packets** these have a "header" id so you can identify what the action should be done on server or client side.

## Create & Run
The constructor of a Server or Client creates a socket by using the windows library.

The Run(float) function will start the listening loop on another thread, so this is **not a blocking call**. The thread will stop working when the destructor is called or if the connection is lost (client side).

Server code:
```cpp
Server server{12345}; //Create server on port 12345
server.Run(20.f); //Run server @ 20 ticks
```
Client code:
```cpp
Client client{ 12345, "127.0.0.1" }; //Create client on port 12345 & connect to server with ip 127.0.0.1
client.Run(20.f); //Run client @ 20 ticks
```

## Packet
Making a packet requires an id for the packet. After that you can write anything to it. Be aware you read it out in the same order as you added it. Reading something will delete the data that it has read.

```cpp
Packet packet{500};
packet.Write(546.5f); //Float value
packet.Write(true); //Bool value
packet.Write(10004); //Int value
```

```cpp
void Packet500Handler(Packet& packet)
{
    //Read data from packet
    const auto health = packet.Read<float>();
    const auto pvp = packet.Read<bool>();
    const auto money = packet.Read<int>();

    //Print data from packet
    std::cout << "health: " << health << "\n";
    std::cout << "pvp: " << pvp << "\n";
    std::cout << "money: " << money << "\n";
}
```


## Bind
With bind you can link a certain header to a function. The function requires the parameter **Packet&**. So, the function can have the data of that packet. Be aware it is a reference. Server binding requires the packet and an **int**. This is necessary for the server logic, so you know what socket send it to you.

Server code:
```cpp
void Example(Packet& packet, int senderId)
{
    std::cout << "Message from client\n";
}
int main()
{
	//Do stuff before
	client.Bind(777, Example); //Bind package with id 777 to function Example
	//Do stuff after
}
```
Client code:
```cpp
void Example(Packet& packet)
{
    std::cout << "Server sent me something\n";
}
int main()
{
	//Do stuff before
	client.Bind(777, Example); //Bind package with id 777 to function Example
	//Do stuff after
}
```
## Send packet
The send function just requires a Packet. For server there are two functions. One to send to all clients and one to send to a specific client.

Server code:
```cpp
Packet packet{ 777 };
server.SendPacketAll(packet); //Send packet to all clients
server.SendPacket(packet, 0); //Send packet to client id 0
```
Client code:
```cpp
Packet packet{ 500 };
client.SendPacket(packet);
```