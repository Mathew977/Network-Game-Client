#pragma once
#include "Entity.h"

class Player
{
private:

	//Stores the number of points the player has
	struct playerSpecificStats
	{
		int points;
	};

	stats playerStats; //Contains all the stats of the player
	playerSpecificStats player;

public:

	//Sets the player information
	void setPlayerInformation(char*, int, float, float, float, int);

	//Update the player's values
	void updatePlayer(int, float, float, int);

	stats getPlayerStats();

	int getPoints();

	Player();
	~Player();
};

