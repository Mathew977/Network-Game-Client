#include "pch.h"
#pragma once
#pragma warning(disable : 4996)

#include "ClassUDPClient.h"
#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <time.h>

using namespace std;

#define DEFAULT_PORT "27015" //Holds the dafult port used by the client and the server
#define DEFAULT_BUFLEN 512 //Holds the maximum buffer length to be used

class ClassTCPClient
{
private:
	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		iResult;
	int		recvbuflen = DEFAULT_BUFLEN;

	struct addrinfo *addressList = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	const char	*serverName;

	SOCKET connectSocket = INVALID_SOCKET;

public:
	char* tcp(); //TCP startup
	void sendMessage(char*); //Handles sending messages
	char* recvMessage(); //Handles Recieving messages

	ClassTCPClient();
	~ClassTCPClient();
};

