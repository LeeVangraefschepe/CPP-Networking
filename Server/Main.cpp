#include "pch.h"
/*#include <array>
#include <iostream>
#include <winsock2.h>//For Windows socket programming
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")//Link with the Winsock library*/
#include "Server.h"

int main()
{
    Server server{12345};
    server.Run(20.f);

    return 0;
}