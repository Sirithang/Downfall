#pragma once 

#include "map_types.h"
#include <map>

typedef void (*CollisionCallback)(void *,unsigned int, unsigned int);
struct Callback16
{
	void (*f)(void*,unsigned int, unsigned int);
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

	struct CollisionArray
	{
		unsigned char _nb;
		Callback16 _callbaks[32];
	};

	std::map<uint32_t,CollisionArray> _callbacks;

	CollisionManager::Collision _collisions[256];

	unsigned char _nbCollisions;
};

namespace collisionmanager
{
	void registerCallback(uint32_t entityID, Callback16 callback);
	void reinit();
	void doCallbacks();
	void testAgaintMap(MapInfo& map);
}