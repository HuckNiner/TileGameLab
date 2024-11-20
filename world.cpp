#include <iostream>
#include <cstdlib>
#include <ctime>
#include "world.h"
#include "player_entity.h"
#include "pubsub.h"
#include "raylib.h"

World::World()
{
	srand(time(NULL));
}

void World::tick()
{
	for (Entity *entity : entities)
		entity->tick();

	// Detect collisions
	collisions.clear();
	for (Entity* entity : entities)
	{
		if (entity->getMoving() || entity->getFalling())
		{
			for (Entity* entity2 : entities)
			{
				if (entity != entity2)
				{
					checkCollision(entity, entity2);
				}
			}
		}
	}

	// Handle collisions
	deletions.clear();
	for (Entity* entity : collisions)
	{
		if (entity->handleCollisions())
			deletions.push_back(entity);
	}

	// Remove deleted entities
	for (Entity* entity : deletions)
	{
		vector<Entity*>::iterator result = find(entities.begin(), entities.end(), entity);

		if (result != entities.end())
			entities.erase(result);

		PubSub::publish("entity", "delete", entity);
		delete entity;
	}


}

void World::checkCollision(Entity* entity, Entity* entity2)
{
	Rectangle rec1 = {entity->getX(), entity->getY(), entity->getWidth(), entity->getHeight()};
	Rectangle rec2 = { entity2->getX(), entity2->getY(), entity2->getWidth(), entity2->getHeight() };

	if (CheckCollisionRecs(rec1, rec2))
	{
		entity->addCollision(entity2);

		if (find(collisions.begin(), collisions.end(), entity) == collisions.end())
			collisions.push_back(entity);

		if (!entity2->getMoving())
		{
			entity2->addCollision(entity);
			if (find(collisions.begin(), collisions.end(), entity2) == collisions.end())
				collisions.push_back(entity2);
		}
	}
}

void World::clearWorld()
{
	for (Entity* entity : entities)
	{
		// Let everyone know we're deleting this entity
		PubSub::publish("entity", "delete", entity);
		delete entity;
	}

	entities.clear();
}

void World::addEntity(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new Entity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

void World::addPlayer(int x, int y, int width, int height, EntityType type)
{
	Entity* entity = new PlayerEntity(x, y, width, height, type);

	// Let everyone know the entity has been created
	PubSub::publish("entity", "new", entity);

	entities.push_back(entity);
}

void World::changeLevel(int levelNumber)
{
	clearWorld();

	if (levelNumber == 1) {

		float x = 320;
		float y = 280;
		addPlayer(64, 512, 32, 32, Cat);

		// Bottom wall
		y = 544;
		for (float x = 32; x <= 608; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top wall
		y = 64;
		for (float x = 32; x <= 608; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Left vertical wall
		x = 32;
		for (y = 96; y <= 512; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Right vertical wall
		x = 608;
		for (y = 96; y <= 512; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top First vertical Wall
		x = 288;
		for (y = 96; y <= 160; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top Second Vertical Wall
		x = 512;
		for (y = 96; y <= 352; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top First Vertical Wall Below and to the Right of it
		x = 320;
		for (y = 256; y <= 540; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Long Horizontal Wall
		y = 256;
		for (x = 128; x <= 416; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Horizontal Wall Further Down
		y = 160;
		for (x = 384; x <= 488; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Horizontal Wall above the other one
		y = 160;
		for (x = 128; x <= 288; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Closest Vertical Wall to Spawn
		x = 128;
		for (y = 288; y <= 448; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// 2nd Closest Vertical Wall to Spawn
		x = 224;
		for (y = 352; y <= 448; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Middle Horizontal Wall
		y = 352;
		for (x = 224; x <= 288; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Bottom Horizontal Wall
		y = 448;
		for (x = 320; x <= 480; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// 3rd Closest Vertical Wall to Spawn
		x = 416;
		for (y = 352; y <= 416; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Place the portal at the end of the clear path
		addEntity(576, 96, 32, 32, Portal);
		addEntity(544, 96, 32, 32, Portal);
		addEntity(96, 512, 32, 32, Portal); //Cheat portal to work on level 2
	}

	else if (levelNumber == 2) {  //if world equals 2
		float x = 320;
		float y = 280;
		addPlayer(x, y, 32, 32, Cat);

		// Bottom wall
		y = 544;
		for (float x = 32; x <= 608; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top wall
		y = 64;
		for (float x = 32; x <= 608; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Left vertical wall
		x = 32;
		for (y = 96; y <= 512; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Right vertical wall
		x = 608;
		for (y = 96; y <= 512; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top First vertical Wall
		x = 288;
		for (y = 96; y <= 160; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top Second Vertical Wall
		x = 384;
		for (y = 96; y <= 128; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top Second Vertical Wall Split
		x = 512;
		for (y = 256; y <= 352; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Top First Vertical Wall Below and to the Left of it
		x = 224;
		for (y = 256; y <= 540; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Long Horizontal Wall
		y = 256;
		for (x = 32; x <= 128; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Long Horizontal Wall Split
		y = 256;
		for (x = 224; x <= 480; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Horizontal Wall Further Down
		y = 160;
		for (x = 384; x <= 512; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Horizontal Wall above the other one
		y = 160;
		for (x = 64; x <= 192; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Closest Vertical Wall to Spawn
		x = 128;
		for (y = 288; y <= 448; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Middle Horizontal Wall
		y = 352;
		for (x = 256; x <= 320; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Bottom Horizontal Wall
		y = 448;
		for (x = 320; x <= 480; x += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Third Bottom Vertical Wall
		x = 416;
		for (y = 288; y <= 416; y += 32)
			addEntity(x, y, 32, 32, Obstacle);

		// Place the portal at the end of the clear path
		addEntity(64, 512, 32, 32, Portal);
		addEntity(96, 512, 32, 32, Portal);
		addEntity(352, 312, 32, 32, Portal);


	}

}



