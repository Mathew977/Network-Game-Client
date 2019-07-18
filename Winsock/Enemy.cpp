#include "pch.h"
#include "Enemy.h"

#pragma warning(disable : 4996)

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

//Set the enemy information
void Enemy::setEnemyInformation(char* eName, int hp, float x, float y, float speed)
{
	strcpy(enemyStats.name, eName); //Set the enemy's name
	enemyStats.health = hp; //Set the enemy's health
	this->enemyStats.x = x; //Set the enemy's x position
	this->enemyStats.y = y; //Set the enemy's y position
	this->enemyStats.speed = speed; //Set the enemy's speed
}

//Updates the enemy's values
void Enemy::updateEnemy(int hp, float x, float y)
{
	enemyStats.health = hp; //Update the enemy's health
	this->enemyStats.x = x; //Update the enemy's x position
	this->enemyStats.y = y; //Update the enemy's y position
}

stats Enemy::getEnemyStats()
{
	return enemyStats; //Return 
}