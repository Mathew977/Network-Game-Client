#include "pch.h"
#include "ClassUDPClient.h"

#pragma warning(disable : 4996)

ClassUDPClient::ClassUDPClient()
{
}


ClassUDPClient::~ClassUDPClient()
{
}

unsigned short ClassUDPClient::_PackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
{
	return(PackPreamble(preamb, buf)); //Calls the function which packs the preamble message and returns the length of the message
}

unsigned short ClassUDPClient::_UnpackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
{
	return(UnpackPreamble(preamb, buf)); //Calls the function which unpacks the preamble message and returns the length of the message
}

void ClassUDPClient::_UnpackPlayer(PLAYER_INFORMATION *player, unsigned char *buf)
{
	UnpackPlayer(player, buf); //Calls the function which unpacks the player message
}

void ClassUDPClient::_UnpackEnemy(ENEMY_INFORMATION *enemy, unsigned char *buf)
{
	UnpackEnemy(enemy, buf); //Calls the function which unpacks the enemy message
}

unsigned short ClassUDPClient::_PackMovement(PLAYER_MOVEMENT_INFORMATION *move, unsigned char *buf)
{
	return(PackMovement(move, buf)); //Calls the function which packs the movement message and returns the length of the message
}

unsigned short ClassUDPClient::_PackBullet(MESSAGE_BULLET_FIRED *bullet, unsigned char *buf)
{
	return(PackBullet(bullet, buf)); //Calls the function which packs the bullet message and returns the length of the message
}

void ClassUDPClient::_UnpackBullet(MESSAGE_BULLET_FIRED *bullet, unsigned char *buf)
{
	UnpackBullet(bullet, buf); //Calls the function which unpacks the bullet message
}

void ClassUDPClient::udp(char* character)
{
	//Do this only once at startup
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

	if (iResult != 0)
	{
		printf("WSAStartup failed: %d\n", iResult);
	}
}

//Handles the bullet messages
void ClassUDPClient::sendMousePress(int code, char* user, int x, int y, int count)
{
	unsigned char sendbuf[DEFAULT_BUFLEN]; //Stores the message to be sent
	unsigned short buflen; //Stores the length of the message to be sent

	MESSAGE_BULLET_FIRED msg; //msg to store the information to be sent

	msg.code = code; //Set the msg's code
	strcpy((char*)msg.username, user); //Set the msg's username
	snprintf(msg.bulletX, MAX_MESSAGE_CHAR_LENGTH, "%d", x); //Set the msg's bulletX value
	snprintf(msg.bulletY, MAX_MESSAGE_CHAR_LENGTH, "%d", y); //Set the msg's bulletY value
	msg.count = count; //Set the msg's count

	//Pack the message structure into the send buffer.
	buflen = PackBullet(&msg, (unsigned char*)sendbuf);

	//Send the information to be sent in a preamble to the sendPreamble function
	sendPreamble(code, buflen);

	//Send the sendbuf and the buflen to the sendMessage function
	sendMessage(sendbuf, (int)buflen);
}

//Handles the move messages
void ClassUDPClient::sendKeyboardPress(int code, char* user, char* movement)
{
	unsigned char sendbuf[DEFAULT_BUFLEN]; //Stores the message to be sent
	unsigned short buflen; //Stores the length of the message to be sent

	PLAYER_MOVEMENT_INFORMATION msg; //msg to store the information to be sent

	msg.code = code; //Set the msg's code
	strcpy((char*)msg.username, user); //Set the msg's username
	strcpy((char*)msg.input, movement); //Set the msg's input

	//Pack the message structure into the send buffer.
	buflen = PackMovement(&msg, (unsigned char*)sendbuf);

	//Send the information to be sent in a preamble to the sendPreamble function
	sendPreamble(code, buflen);

	//Send the sendbuf and the buflen to the sendMessage function
	sendMessage(sendbuf, (int)buflen);
}

//Handles the preamble messages
void ClassUDPClient::sendPreamble(int code, int buflen)
{
	MESSAGE_PREAMBLE preMsg; //msg to store the information to be sent

	unsigned char presendbuf[DEFAULT_BUFLEN]; //Stores the message to be sent
	unsigned short prebuflen; //Stores the length of the message to be sent

	preMsg.code = code; //Set the preMsg's code - This tells the server what the next message it will recieve is going to be
	preMsg.length = buflen; //Set the preMsg's buflen

	//Pack the preMsg structure into the presendbuffer
	prebuflen = (PackPreamble(&preMsg, (unsigned char*)presendbuf));

	//Send the sendbuf and the buflen to the sendMessage function
	sendMessage(presendbuf, (int)prebuflen);
}

//Handles sending the messages
void ClassUDPClient::sendMessage(unsigned char* sendbuf, int buflen)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	//Call the getaddrinfo function requesting the IP address for the 
	//server name passed on the command line
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &addressList);

	//Check if it failed to get the addressinfo
	if (iResult != 0)
	{
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
	}

	//Check if the addressList doesn't have a value
	if (addressList != NULL)
	{
		//Set up the UdpSocket
		UdpSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);

		//Check if the UdpSocket is invalid
		if (UdpSocket == INVALID_SOCKET)
		{
			printf("Error at socket(): %ld\n", WSAGetLastError());
			clearAdr();
			WSACleanup();
		}
	}

	//Send the message to the server
	iResult = sendto(UdpSocket, (char *)sendbuf, buflen, 0, addressList->ai_addr, (int)addressList->ai_addrlen);

	//Check if the send failed
	if (iResult == SOCKET_ERROR)
	{
		printf("send failed: %d\n", WSAGetLastError());
		clearAdr();
		closesocket(UdpSocket);
		WSACleanup();
	}
}

//Handles recieving the messages
char* ClassUDPClient::recvMessage()
{
	//Recieve the message from the server
	iResult = recvfrom(UdpSocket, recvbuf, recvbuflen, 0, NULL, NULL);

	//Check if the client failed to recieve the message
	if (iResult == SOCKET_ERROR)
	{
		printf("recv failed: %d\n", WSAGetLastError());
		closesocket(UdpSocket);
		WSACleanup();
	}
	else
	{
		printf("Bytes received: %d Msg=%s\n", iResult, recvbuf);
	}

	return recvbuf; //Return the message
}

//Handles freeing the address info
void ClassUDPClient::clearAdr()
{
	freeaddrinfo(addressList); //Avoid memory leak
}

void ClassUDPClient::closeUDP()
{
	//Cleanup
	closesocket(UdpSocket);

	//Do this only once at the end
	WSACleanup();
}