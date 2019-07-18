#pragma once
#define BULLETCOUNT 10 //Set the max number of bullets
#include "Entity.h"

class Bullet
{
private:
	stats enemyBulletStats[BULLETCOUNT]; //Stores an array of information for the enemy's bullets
	stats bulletStats[BULLETCOUNT]; //Stores an array of information for the Player's bullets

	int bulletCounter = 0;
public:
	void updateBullet(char*, float, float, int); //Handles updating the bullet's positions

	float getBulletX(int); //Returns the x coordinate of the bullet
	float getBulletY(int); //Returns the y coordinate of the bullet

	stats getBulletStats(int);
	stats getEnemyBulletStats(int);

	Bullet();
	~Bullet();
};

