#pragma once 

#include "map_types.h"

typedef void (*CollisionCallback)(void *, unsigned int);
struct Callback16
{
	void (*f)(void*, unsigned int);
	char data[12];
};

struct CollisionManager
{
	struct Collision
	{
		uint32_t _colliderID;
		uint32_t _collideeID;
		Callback16 _callback;
	};

	CollisionManager::Collision _collisions[256];
	unsigned char _nbCollisions;
};

namespace collisionmanager
{
	void reinit();
	void doCallbacks();
	void testAgaintMap(MapInfo& map);
}