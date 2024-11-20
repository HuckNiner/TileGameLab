#include <iostream>
#include "raylib.h"
#include "player_entity.h"
#include "world.h"

#include <iostream>

PlayerEntity::PlayerEntity(float x, float y, int width, int height, EntityType type)
	: Entity(x, y, width, height, type)
{
	PubSub::subscribe("action", this);

	movementCountdown = 0;
	targetX = 0;
	targetY = 0;
	speedX = 0;
	speedY = 0;
}
PlayerEntity::~PlayerEntity() {
	PubSub::unsubscribe("action", this);

}

void PlayerEntity::tick()
{
	if (movementCountdown > 0)
	{
		movementCountdown--;

		if (movementCountdown == 0)
		{
			setMoving(false);
			setX(targetX);
			setY(targetY);
			
		}
		else
		{
			setX(getX() + speedX);
			setY(getY() + speedY);
			
		}
	}
	else {
		
	}

}

void PlayerEntity::receiveMessage(string channel, string message, void* data)
{
	if (channel == "action" && message == "player")
	{
		PlayerAction* action = (PlayerAction*)data;

		if (*action == PlayerRight)
		{
			targetX = getX() + 4;
			targetY = getY();
			speedX = 32.0 / 60.0;
			speedY = 0;
			setMoving(true);
			setFacing(Right);
			movementCountdown = 4;
		}

		if (*action == PlayerLeft)
		{
			targetX = getX() - 4;
			targetY = getY();
			speedY = 0;
			speedX = -32.0 / 60.0;
			setMoving(true);
			setFacing(Left);
			movementCountdown = 4;
		}

		if (*action == PlayerDown)
		{
			targetX = getX();
			targetY = getY() + 4;
			speedY = 32.0 / 60.0;
			speedX = 0;
			setMoving(true);
			setFacing(Down);
			movementCountdown = 4;
		}

		if (*action == PlayerUp)
		{
			targetX = getX();
			targetY = getY() - 4;
			speedY = -32.0 / 60.0;
			speedX = 0;
			setMoving(true);
			setFacing(Up);
			movementCountdown = 4;
		}

	}
}

bool PlayerEntity::handleCollisions()
{
	for (Entity* entity : collisions)
	{
		if (entity->getType() == Obstacle)
		{
			int xDistance = abs(getX() - entity->getX());
			int yDistance = abs(getY() - entity->getY());

			if (xDistance > yDistance)
			{
				setMoving(false);
				movementCountdown = 0;

				setX(getX() - speedX);
				setY(getY() - speedY);
			}
			
		}
		else if (entity->getType() == Portal) {
			PubSub::publish("entity", "changingLevel", this);
		}
	}

	collisions.clear();
	return false;
}
