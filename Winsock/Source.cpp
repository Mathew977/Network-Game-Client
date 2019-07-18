#include "pch.h"
#include "ClassTCPClient.h"
#include "ClassUDPClient.h"
#include "Messages.h"
#include "PackUnpack.h"
#include "glut.h"
#include "Entity.h"
#include "Enemy.h"
#include "Bullet.h"
#include <Windows.h>

#pragma warning(disable : 4996)

#define SCREEN_HEIGHT 500 //Glut Window Height
#define SCREEN_WIDTH 500 //Glut Window Width

ClassTCPClient tcp;
ClassUDPClient udp;
Player player;
Entity entity;
Enemy enemy;
Bullet bullet;

char name[DEFAULT_BUFLEN]; //Stores the Players Name
char enemyName[DEFAULT_BUFLEN]; //Stores the Enemy's Name


void gameLoop(int, char**); //Handles the opengl game
void setup(); //Initialises all the values for the game
void update(); //Recieves UDP Messages

bool endInit = false; //Determines if the Initialisation is finished

int enemyBulletCount = 0; //Holds the next bullet to fire for the enemy
int enemyBulCount = 0; //Holds the number of bullets fired for the enemy

int bulletCount = 0; //Holds the next bullet to fire for the player
int bulCount = 0; //Holds the number of bullets fired for the player

int terminate_app = 0;

#define MAXTHREADS 1 //number of Extra threads being used

LPCTSTR mutexname = (LPCTSTR)"FirstMutex";

//Thread handles recieving messages from the server and Redisplaying the opengl scene
DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	DWORD param = (DWORD)lpParameter; //Recover passed in value
	int index;
	HANDLE  mutexhdl;

	//Open the mutexhdl
	mutexhdl = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);

	//Loop forever
	while (1)
	{
		WaitForSingleObject(mutexhdl, INFINITE); //Wait until the mutex can be used
		update(); //Call the update function that recieves messages
		ReleaseMutex(mutexhdl); //Release the mutex

		glutPostRedisplay(); //Redisplay the game scene
	}

	return 0;
}

//Overview of all the steps in the program for the client
int main(int argc, char* argv[])
{
	strcpy(name, tcp.tcp()); //Runs the tcp client and returns the name of the player
	udp.udp(name); //Run the udp client and passes in the name of the player

	setup(); //Initialises all the values for the game by using values recieved from the server

	gameLoop(argc, argv); //Enter the main game loop

	udp.closeUDP(); //Close the UDP Client

	return 0; //Return 0
}

//Displays the game
void drawMap()
{
	//Clear the buffer
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(10); //Set the point sizes to 10

	//DRAW PLAYER
	glBegin(GL_POINTS);
	glColor3f(0, 1, 0); //Set the colour of the player to green
	
	//Draw the player in the scene
	glVertex2f(entity.getX(player.getPlayerStats()), entity.getY(player.getPlayerStats()));
	glEnd();
	//DRAW PLAYER

	//DRAW THE PLAYER'S BULLETS
	//Check if the player has fired a bullet yet
	if (bulletCount > 0)
	{
		glPointSize(4); //Set the point sizes to 4

		//Loop through for the number of bullets the player has created
		for (int i = 0; i < bulCount; i++)
		{
			glBegin(GL_POINTS);
			glColor3f(0, 1, 0); //Set the player's bullets to green
			
			//Draw the player's bullets in the scene
			glVertex2f(entity.getX(bullet.getBulletStats(i)), entity.getY(bullet.getBulletStats(i)));
			glEnd();
		}
	}
	//DRAW THE PLAYER'S BULLETS

	//DRAW THE ENEMY'S BULLETS
	//Check if the enemy has fired a bullet yet
	if (enemyBulletCount > 0)
	{
		glPointSize(4); //Set the points sizes to 4

		//Loop through for the number of bullet the enemy has created
		for (int j = 0; j < enemyBulCount; j++)
		{
			glBegin(GL_POINTS);
			glColor3f(1, 0, 0); //Set the enemy's bullets to red
			
			//Draw the enemy's bullets in the scene
			glVertex2f(entity.getX(bullet.getEnemyBulletStats(j)), entity.getY(bullet.getEnemyBulletStats(j)));
			glEnd();
		}
	}
	//DRAW THE ENEMY'S BULLETS

	//DRAW THE ENEMY
	glPointSize(10); //Set the points sizes to 10

	glBegin(GL_POINTS);
	glColor3f(1, 0, 0); //Set the colour of the enemy to red

	//Draw the enemy in the scene
	glVertex2f(entity.getX(enemy.getEnemyStats()), entity.getY(enemy.getEnemyStats()));
	glEnd();
	//DRAW THE ENEMY

	glFlush();

	glutSwapBuffers();
}

//Handles keyboard presses when the game scene is running
void keyboard(unsigned char key, int x, int y)
//This function alters the x,y position
{
	HANDLE  mutexhdl;

	//Open mutex mutexhdl
	mutexhdl = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);

	char move[MAX_MESSAGE_CHAR_LENGTH]; //Holds the player's keyboard input

	//Check if the user input 'd' or 'a' as those are the only movements the player may make
	if (key == 'd' || key == 'D')
		strcpy(move, "d"); //Set move to 'd'
	else if (key == 'a' || key == 'A')
		strcpy(move, "a"); //Set move to 'a'

	WaitForSingleObject(mutexhdl, INFINITE); //Wait until the mutex can be used
	
	//Send the player move code, the player name and the move 
	//input to the send keyboardpress function in the udp class
	udp.sendKeyboardPress(MESSAGE_CODE_PLAYER_MOVE, name, move); 

	ReleaseMutex(mutexhdl); //Release the mutex
}

//Handles the player mouse clicks
void mouseClick(int button, int state, int x, int y)
{
	HANDLE  mutexhdl;

	//Open mutex mutexhdl
	mutexhdl = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);

	//Check if the left button has been pressed
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//Check if the last bullet fired was bullet number 10
		if (bulletCount == BULLETCOUNT)
			bulletCount = 0; //Reset the count to bullet number 1
		
		WaitForSingleObject(mutexhdl, INFINITE);//Wait until the mutex can be used

		//Send the bullet fired code, the player name,
		//the mouse click coordinates and the bulletCount
		udp.sendMousePress(MESSAGE_CODE_BULLET_FIRED, name, x, y, bulletCount);
		
		bulletCount++; //Increment the bulletCount by 1
		bulCount++; //Increment the bulCount by 1

		//Check if the bulCount is >= to the max number of bullets
		if (bulCount >= BULLETCOUNT)
			bulCount = BULLETCOUNT; //Set the bulCount to be equalt to the max number of bullets
		
		ReleaseMutex(mutexhdl); //Release the mutex
	}
}

//Main game loop
void gameLoop(int argc, char* argv[])
{
	HANDLE  threadhdl[MAXTHREADS];
	DWORD   threadid;
	int     index = 0;
	DWORD	param = 2;  // Some value to pass to thread

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutCreateWindow("PVE Gamemode");
	glutKeyboardFunc(keyboard); //Handles Keyboard presses
	glutMouseFunc(mouseClick); //Handles Mouse presses
	glutDisplayFunc(drawMap); //Draw the scene
	glOrtho(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT, 0.0, 1.0); //Game Camera

	threadhdl[0] = CreateThread(
		NULL,               // No security attributes reqd
		0,                  // Use default thread stack size
		ThreadProc,         // Thread function
		(LPVOID)param,      // Pass parameter to function
		0,                  // Creation flags (e.g. CREATE_SUSPENDED)
		&threadid);         // Return value of thread ID

	glutMainLoop();

	//TerminateThread(threadhdl, 0); // Kill thread
	terminate_app = 1;

#if MAXTHREADS > 1
	WaitForMultipleObjects(MAXTHREADS),
		threadhdl,
		TRUE,
		INFINITE);
#else
	WaitForSingleObject(threadhdl[0], INFINITE);
#endif // MAXTHREADS > 1

	for (index = 0; index < MAXTHREADS; index++)
	{
		CloseHandle(threadhdl[index]); //Close thread
	}

	getchar(); //Pause
}

//Handles setting up the connections between the client and the server
//Handles initialising the game
void setup()
{
	//Send the first message from the client 
	//to the server to establish the connection
	udp.sendPreamble(0, 0);

	//Call the udp function that clears the address
	udp.clearAdr();

	do
	{
		update(); //Call the update function that waits to recieve a message
	} while (!endInit); //Loop until Initialisation is complete
}

//Handles recieving messages
void update()
{
	//Stores the recieved preamble message
	unsigned char* recvdbuf = (unsigned char*)udp.recvMessage();

	MESSAGE_PREAMBLE preMsg; //Stores the preambles information

	udp._UnpackPreamble(&preMsg, recvdbuf); //Unpack the preamble message

	printf("\nPreamble Code: %d\n", preMsg.code);

	//Do something depending on what the preambles code is
	switch (preMsg.code)
	{
	//Check if the preamble code matches the player update code
	case MESSAGE_CODE_PLAYER_UPDATE:
	{
		PLAYER_INFORMATION playerMsg; //Stores the player's recieved information

		//Stores the recieved player update message
		unsigned char* recvdbuf = (unsigned char*)udp.recvMessage();

		//Unpack the player message
		udp._UnpackPlayer(&playerMsg, recvdbuf);

		//Print out the details of the message
		printf("Message Recieved:\n Code: %d\n Character: %s\n Health: %d\n X Position: %f\n Y Position: %f\n Speed: %f\n Points: %d\n",
			playerMsg.code, playerMsg.username, playerMsg.health, atof(playerMsg.startX),
			atof(playerMsg.startY), atof(playerMsg.speed), playerMsg.points);

		//Send the contents of the message to the player's
		//setPlayerInformation function to be stored
		player.setPlayerInformation(name, playerMsg.health, atof(playerMsg.startX), atof(playerMsg.startY), atof(playerMsg.speed), playerMsg.points);

		break;
	}
	//Check if the preamble code matches the enemy update code
	case MESSAGE_CODE_ENEMY_UPDATE:
	{
		ENEMY_INFORMATION enemyMsg;  //Stores the enemy's recieved information

		//Stores the recieved enemy update message
		unsigned char* recvdbuf = (unsigned char*)udp.recvMessage();
		
		//Unpack the enemy message
		udp._UnpackEnemy(&enemyMsg, recvdbuf);

		//Print out the details of the message
		printf("Message Recieved:\n Code: %d\n Character: %s\n Health: %d\n X Position: %f\n Y Position: %f\n Speed: %f\n",
			enemyMsg.code, enemyMsg.username, enemyMsg.health, atof(enemyMsg.startX),
			atof(enemyMsg.startY), atof(enemyMsg.speed));

		//Set the enemy's name to the name recieve in the enemy message
		strcpy(enemyName, enemyMsg.username);

		//Send the contents of the message to the enemy's
		//setEnemyInformation function to be stored
		enemy.setEnemyInformation(enemyName, enemyMsg.health, atof(enemyMsg.startX), atof(enemyMsg.startY), atof(enemyMsg.speed));

		endInit = true;
		break;
	}
	//Check if the preamble code matches the bullet fired code
	case MESSAGE_CODE_BULLET_FIRED:
	{
		MESSAGE_BULLET_FIRED bulletMsg; //Stores the bullet's recieved information

		//Stores the recieved bullet fired message
		unsigned char* recvdbuf = (unsigned char*)udp.recvMessage();

		//Unpack the bullet message
		udp._UnpackBullet(&bulletMsg, recvdbuf);

		if (bulletMsg.code != MESSAGE_CODE_BULLET_FIRED)
		{
			printf("RECIEVED ERROR");
		}
		else
		{
			//Print out the details of the message
			printf("Message Recieved:\n Code: %d\n Character: %s\n Bullet X Position: %f\n Bullet Y Position: %f\n Bullet Number: %d\n",
				bulletMsg.code, bulletMsg.username, atof(bulletMsg.bulletX), atof(bulletMsg.bulletY), bulletMsg.count);

			char buffer[MAX_MESSAGE_CHAR_LENGTH]; //Used to store either "Player" if the bullet is the player's or "Enemy" if the bullet is the enemies

			//Check if the name recieved in the message matches player's name
			if (strcmp(bulletMsg.username, name) == 0)
			{
				strcpy(buffer, "Player"); //Set buffer to equal "Player"

				//Send the contents of the message to the bullet's
				//updateBullet function to be stored
				bullet.updateBullet(buffer, atof(bulletMsg.bulletX), atof(bulletMsg.bulletY), bulletMsg.count);
			}
			//Check if the name recieved in the message matches enemy's name
			else if (strcmp(bulletMsg.username, enemyName) == 0)
			{
				
				strcpy(buffer, "Enemy"); //Set buffer to equal "Enemy"

				if (enemyBulletCount == BULLETCOUNT)
					enemyBulletCount = 0;

				//Send the contents of the message to the bullet's
				//updateBullet function to be stored
				bullet.updateBullet(buffer, atof(bulletMsg.bulletX), atof(bulletMsg.bulletY), bulletMsg.count);
				
				enemyBulletCount++;  //Increment the enemyBulletCount by 1
				enemyBulCount++;  //Increment the enemyBulCount by 1

				//Check if the enemyBulCount is >= to the max number of bullets
				if (enemyBulCount >= BULLETCOUNT)
					enemyBulCount = BULLETCOUNT; //Set the bulCount to be equalt to the max number of bullets
			}
				
		}
		break;
	}
	default:
		break;
	}
}