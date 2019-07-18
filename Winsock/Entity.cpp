#include "pch.h"
#include "Entity.h"

#pragma warning(disable : 4996)

Entity::Entity()
{
}

Entity::~Entity()
{
}

int Entity::getHealth(stats stat)
{
	return stat.health; //Return the entity's health
}

float Entity::getX(stats stat)
{
	return stat.x; //Return the entity's x position
}

float Entity::getY(stats stat)
{
	return stat.y; //Return the entity's y position
}

float Entity::getSpeed(stats stat)
{
	return stat.speed; //Return the entity's speed
}