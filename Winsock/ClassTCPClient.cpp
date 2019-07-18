#include "pch.h"
#include "ClassTCPClient.h"

#pragma warning(disable : 4996)

ClassTCPClient::ClassTCPClient()
{
}


ClassTCPClient::~ClassTCPClient()
{
}

//Handles tcp menu
char* ClassTCPClient::tcp()
{
	//Set the IP that the client will connect to
	serverName = "127.0.0.1";

	//Do this only once at startup
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	char sendbuf[DEFAULT_BUFLEN]; //Stores the message to best to the server
	char recvbuf[DEFAULT_BUFLEN]; //Stores the message recieved from the server

	int messageCounter = 0; //Used to tell the server what message you're replying to
	bool reset; //Determines if the user needs to attempt to sign in again
	bool print; //Determines if the message needs to be printed out
	char tempMessage[DEFAULT_BUFLEN]; //Temporarily holds the recvbuf

	char character[DEFAULT_BUFLEN]; //Stores the player's name

	//Loop forever
	while (1)
	{
		print = true; //Set print to true
		reset = false; //Set reset to false

		//Check if messageCounter is equal to 1 and the client needs to establish the connection with the server
		if (messageCounter == 0)
		{
			messageCounter++; //Increment the messageCounter by 1

			strcpy(sendbuf, "login1"); //Set the sendbuf to "login1" which is the message that tells the server to begin the tcp

			sendMessage(sendbuf); //Send the sendbuf to the server

		}
		else
		{
			do
			{
				strcpy(recvbuf, recvMessage()); //Set the recvbuf to the message recieved from the server
			} while (strcmp(recvbuf, "Error") == 0 || strcmp(recvbuf, "Server Closed") == 0); //Loop until the correct message has been recieved from the server

			//Check if the recieved message is "Reset"
			if (strcmp(recvbuf, "Reset") == 0)
			{
				messageCounter = 0; //Set the messageCounter to 0 to reset the the loop
				print = false; //Set print to false
				reset = true; //Set reset to true
			}

			//Grab the last character of the message recieved from the server
			char* last_letter = &recvbuf[strlen(recvbuf) - 1];

			//Check if the last character in the message is equalt to 4
			if (strcmp(last_letter, "4") == 0)
			{
				int i;

				//Loop through for how long the message recieved is
				for (i = 0; i < (int)strlen(recvbuf) - 1; i++)
				{
					tempMessage[i] = recvbuf[i]; //Set the tempMessage to be the same as the recieved message
				}

				tempMessage[i] = '\0'; //Add the terminating character to the end of tempMessage

				int messageNumber = atoi(last_letter); //Set messageNumber to be equal to the last character, which in this case is 4

				messageCounter = messageNumber - 1; //Subtract one from the messageCounter

				printf("%s", tempMessage); //Print out the message recieved from the server
			}
			else
			{
				printf("%s", recvbuf); //Print out the message recieved from the server
			}

			//Check if reset is false and the message recieved from the server isn't "Start Game"
			if (!reset && strcmp(recvbuf, "Start Game") != 0)
			{
				messageCounter++; //Increment messageCounter by 1

				scanf("%s", sendbuf); //Let the user input the characters name

				//check if the messageCounter is 2
				if (messageCounter == 2)
				{
					strcpy(character, sendbuf); //Set the character variable to the same as the input from the user
				}

				sprintf(sendbuf, "%s%d", sendbuf, messageCounter); //Set the sendbuf to be equal to itself with messageCounter on the end

				sendMessage(sendbuf); //Pass the sendbuf through to the sendMessage function to be sent to the server
			}
		}
		system("CLS"); //Clear the console

		//Check if the message recieved is "Start Game"
		if (strcmp(recvbuf, "Start Game") == 0)
		{
			WSACleanup();
			return character; //Exit TCP and return the player's name
		}
	}
}

//Handles sending the message from the client to the server
void ClassTCPClient::sendMessage(char* msg)
{
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	//Call the getaddrinfo function requesting the IP address for the 
	//server name passed on the command line
	getaddrinfo(serverName, DEFAULT_PORT, &hints, &addressList);

	//Set the connectSocket
	connectSocket = socket(addressList->ai_family, addressList->ai_socktype, addressList->ai_protocol);

	//Active open
	connect(connectSocket, addressList->ai_addr, (int)addressList->ai_addrlen);

	freeaddrinfo(addressList); //Avoid memory leak

	//Send the msg to the server
	iResult = send(connectSocket, msg, (int)strlen(msg) + 1, 0);

	//Print out the iResult
	printf("Bytes sent: %ld\n", iResult);

	//Shitdown the socket
	shutdown(connectSocket, SD_SEND);
}

//Handles recieving the message
char* ClassTCPClient::recvMessage()
{
	char message[DEFAULT_BUFLEN]; //Holds the message recieved from the server

	//BLOCK waiting to recieve reponse from server
	//Set recvbuf to the message being recieved 
	iResult = recv(connectSocket, recvbuf, recvbuflen, 0);

	//Check if iResult is greater than 0
	if (iResult > 0)
	{
		//iResult is the number of bytes received
		//Assumes NUL char is included in recvbuf
		//to terminate the string
		int i;

		//Loop through for the number of characters in the recvbuf
		for (i = 0; i < (int)strlen(recvbuf); i++)
		{
			message[i] = recvbuf[i]; //Set the message to be the same as the recvbuf
		}
		message[i] = '\0'; //Add the terminating character to the end of message
	}
	else if (iResult == 0) //Check if iResult is equal to 0
	{
		printf("Connection closed by server");
		strcpy(message, "Server Closed"); //Set message to "Server Closed"
	}
	else
	{
		printf("recv failed: %d\n", WSAGetLastError());
		strcpy(message, "Error"); //Set message to "Error"
	}

	//Close the socket
	closesocket(connectSocket);

	return message; //Return the message recieved from the server
}