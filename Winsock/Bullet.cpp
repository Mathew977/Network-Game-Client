#include "pch.h"
#include "Bullet.h"

#pragma warning(disable : 4996)

//Set the start values of all the bullets
Bullet::Bullet()
{
	//Loop through for the total number of bullets
	for (int i = 0; i < BULLETCOUNT; i++)
	{
		strcpy(bulletStats[i].name, "Player"); //Set the player bullet's name to "Player" to identify it as the player's bullet
		bulletStats[i].x = -255; //Set the player bullet's x coordinate to null
		bulletStats[i].y = -255; //Set the player bullet's y cordinate to null

		strcpy(enemyBulletStats[i].name, "Enemy"); //Set the player bullet's name to "Enemy" to identify it as the player's bullet
		enemyBulletStats[i].x = -255; //Set the enemy bullet's x cordinate to null
		enemyBulletStats[i].y = -255; //Set the enemy bullet's y cordinate to null
	}
}

Bullet::~Bullet()
{
}

//Update the player bullets and enemy bullets positions
void Bullet::updateBullet(char* name, float x, float y, int count)
{
	//Check if the bullet being updated is a player bullet
	if (strcmp(name, "Player") == 0)
	{
		strcpy(bulletStats[count].name, name); //Update the player bullet's name
		bulletStats[count].x = x; //Update the x coordinate of the player's bullet
		bulletStats[count].y = y; //Update the y coordinate of the player's bullet
	}
	//Check if the bullet being updated is an enemy bullet
	else if (strcmp(name, "Enemy") == 0)
	{
		strcpy(enemyBulletStats[count].name, name); //Update the enemy bullet's name
		enemyBulletStats[count].x = x; //Update the x coordinate of the enemy's bullet
		enemyBulletStats[count].y = y; //Update the y coordinate of the enemy's bullet
	}
}

float Bullet::getBulletX(int count)
{
	return bulletStats[count].x; //Return the x coordinate of the bullet currently being looked at
}

float Bullet::getBulletY(int count)
{
	return bulletStats[count].y; //Return the y coordinate of the bullet currently being looked at
}

stats Bullet::getBulletStats(int count)
{
	return bulletStats[count]; //Return the stats of the current player bullet being looked at
}

stats Bullet::getEnemyBulletStats(int count)
{
	return enemyBulletStats[count]; //Return the stats of the current enemy bullet being looked at
}