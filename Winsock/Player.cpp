#include "pch.h"
#include "Player.h"

#pragma warning(disable : 4996)

Player::Player()
{
}


Player::~Player()
{
}

//Set the player information
void Player::setPlayerInformation(char* cName, int hp, float x, float y, float speed, int points)
{
	strcpy(playerStats.name, cName); //Set the player's name
	playerStats.health = hp; //Set the player's health
	this->playerStats.x = x; //Set the player's x position
	this->playerStats.y = y; //Set the player's y position
	this->playerStats.speed = speed; //Set the player's speed
	this->player.points = points; //Set the player's points
}

//Updates the player's values
void Player::updatePlayer(int hp, float x, float y, int points)
{
	playerStats.health = hp; //Update the player's health
	this->playerStats.x = x; //update the player's x position
	this->playerStats.y = y; //Update the player's y position
	this->player.points = points; //Update the player's points
}

stats Player::getPlayerStats()
{
	return playerStats; //Return the playerStats
}

int Player::getPoints()
{
	return player.points; //Return the player's points
}