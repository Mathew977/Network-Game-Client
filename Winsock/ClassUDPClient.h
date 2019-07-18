#pragma once
#include "Messages.h"
#include "PackUnpack.h"
#include "glut.h"
#include "Player.h"
#include <iostream>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>

#define DEFAULT_PORT "27015" //Holds the dafult port used by the client and the server
#define DEFAULT_BUFLEN 512 //Holds the maximum buffer length to be used

using namespace std;

class ClassUDPClient
{
private:
	//Packs the preamble message
	unsigned short PackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
	{
		PackShort(buf, preamb->code); //Pack the Preamble Code
		PackShort(buf, preamb->length); //Pack the Preamble Length
		return PREAMBLE_LENGTH;
	}

	//Unpacks the preamble message
	unsigned short UnpackPreamble(MESSAGE_PREAMBLE *preamb, unsigned char *buf)
	{
		UnpackShort(buf, preamb->code); //Unpack the Preamble Code
		UnpackShort(buf, preamb->length); //Unpack the Preamble Length
		return PREAMBLE_LENGTH;
	}

	//Unpacks the player message
	void UnpackPlayer(PLAYER_INFORMATION *player, unsigned char *buf)
	{
		UnpackShort(buf, player->code);			//Unpack the Player Code
		UnpackBytes(buf, player->username, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Player Name
		UnpackLong(buf, player->health);		//Unpack the Player's Health
		UnpackBytes(buf, player->startX, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Player's X Position
		UnpackBytes(buf, player->startY, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Player's Y Position
		UnpackBytes(buf, player->speed, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Player's speed
		UnpackShort(buf, player->points);	//Unpack the Player's points
	}

	//Unpacks the enemy message
	void UnpackEnemy(ENEMY_INFORMATION *enemy, unsigned char *buf)
	{
		UnpackShort(buf, enemy->code);			//Unpack the Enemy Code
		UnpackBytes(buf, enemy->username, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Enemy Name
		UnpackLong(buf, enemy->health);		//Unpack the Enemy's Health
		UnpackBytes(buf, enemy->startX, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Enemy's X Position
		UnpackBytes(buf, enemy->startY, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Enemy's Y Position
		UnpackBytes(buf, enemy->speed, MAX_MESSAGE_CHAR_LENGTH);	//Unpack the Enemy's speed
	}

	//Packs the player movement message
	unsigned short PackMovement(PLAYER_MOVEMENT_INFORMATION *move, unsigned char *buf)
	{
		unsigned short msglen;

		PackShort(buf, move->code); //Packs the Movement code
		PackBytes(buf, move->username, MAX_MESSAGE_CHAR_LENGTH); //Packs the Movement Username
		PackBytes(buf, move->input, MAX_MESSAGE_CHAR_LENGTH); //Packs the Movement Input

		//Calculates the length of the message
		msglen = sizeof(unsigned short)
			+ sizeof(move->username)
			+ sizeof(move->input);

		return msglen; //Returns the length of the message
	}

	//Packs the Bullet message
	unsigned short PackBullet(MESSAGE_BULLET_FIRED *bullet, unsigned char *buf)
	{
		unsigned short msglen;

		PackShort(buf, bullet->code); //Packs the Bullet Code
		PackBytes(buf, bullet->username, MAX_MESSAGE_CHAR_LENGTH); //Pack the Bullet Username
		PackBytes(buf, bullet->bulletX, MAX_MESSAGE_CHAR_LENGTH); //Pack the Bullet X
		PackBytes(buf, bullet->bulletY, MAX_MESSAGE_CHAR_LENGTH); //Pack the Bullet Y
		PackShort(buf, bullet->count); //Pack the Bullet Counter

		//Calculates the length of the message
		msglen = sizeof(unsigned short)
			+ sizeof(bullet->username)
			+ sizeof(bullet->bulletX)
			+ sizeof(bullet->bulletY)
			+ sizeof(unsigned short);

		return msglen; //Returns the length of the message
	}

	//Unpacks the bullet message
	void UnpackBullet(MESSAGE_BULLET_FIRED *bullet, unsigned char *buf)
	{
		UnpackShort(buf, bullet->code); //Unpack the Bullet Code
		UnpackBytes(buf, bullet->username, MAX_MESSAGE_CHAR_LENGTH); //Unpack the Bullet Username
		UnpackBytes(buf, bullet->bulletX, MAX_MESSAGE_CHAR_LENGTH); //Unpack the Bullet X
		UnpackBytes(buf, bullet->bulletY, MAX_MESSAGE_CHAR_LENGTH); //Unpack the Bullet Y
		UnpackShort(buf, bullet->count); //Unpack the Bullet Counter
	}

	WSADATA	wsaData;
	char	recvbuf[DEFAULT_BUFLEN];
	int		iResult;
	int		recvbuflen = DEFAULT_BUFLEN;
	SOCKET	UdpSocket = INVALID_SOCKET;

	struct addrinfo *addressList = NULL;
	struct addrinfo hints;
public:

	unsigned short _PackPreamble(MESSAGE_PREAMBLE*, unsigned char*); //Calls the PackPreamble function
	unsigned short _UnpackPreamble(MESSAGE_PREAMBLE*, unsigned char*); //Calls the UnpackPreamble function
	void _UnpackPlayer(PLAYER_INFORMATION*, unsigned char*); //Calls the UnpackPlayer function
	void _UnpackEnemy(ENEMY_INFORMATION*, unsigned char*); //Calls the UnpackEnemy function
	unsigned short _PackMovement(PLAYER_MOVEMENT_INFORMATION*, unsigned char*); //Calls the PackMovement function
	unsigned short _PackBullet(MESSAGE_BULLET_FIRED*, unsigned char*); //Calls the PackBullet function
	void _UnpackBullet(MESSAGE_BULLET_FIRED*, unsigned char*); //Calls the UnpackBullet function

	void udp(char*); //Udp startup

	void sendMousePress(int, char*, int, int, int); //Handles Sending the mouse press message
	void sendKeyboardPress(int, char*, char*); //Handles sending keyboard press messages
	void sendPreamble(int, int); //Handles sending the preamble messages
	void sendMessage(unsigned char*, int); //Handles sending Messages

	char* recvMessage(); //Handles Recieving messages

	void clearAdr(); //Clears the Address
	void closeUDP(); //Closes the UDP Socket

	ClassUDPClient();
	~ClassUDPClient();
};

