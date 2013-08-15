#include "core/collisionmanager.h"
#include "component/spherecollider.h"

CollisionManager _manager;

void collisionmanager::testAgaintMap(MapInfo& map)
{
	uint32_t nb = 0;
	SphereCollider* spheres = SphereColliderManager::getDataPtr(nb);

	foundation::Array<CollisionManager::Collision> infos = spherecollider::testAgainstLines(spheres, map._lines._data, nb, map._lines._size);
	int s = foundation::array::size(infos);
	for(int i = 0; i < s; ++i)
	{
		std::map<uint32_t, CollisionManager::CollisionArray>::iterator it = _manager._callbacks.find(infos[i]._colliderID);

		if(it == _manager._callbacks.end())
			continue;

		CollisionManager::CollisionArray& collArray = it->second;

		for(int j = 0; j < collArray._nb; ++j)
		{
			_manager._collisions[_manager._nbCollisions] = infos[i];
			_manager._collisions[_manager._nbCollisions]._callback = collArray._callbaks[j];
			_manager._nbCollisions += 1;
		}
	}
}

void collisionmanager::doCallbacks()
{
	for(int i = 0; i < _manager._nbCollisions; ++i)
	{
		CollisionManager::Collision& coll = _manager._collisions[i];
		CollisionCallback c = (CollisionCallback)coll._callback.f;
		c(coll._callback.data, coll._colliderID, coll._collideeID);
	}
}

void collisionmanager::reinit()
{
	_manager._nbCollisions = 0;
}

void collisionmanager::registerCallback(uint32_t entityID, Callback16 callback)
{
	if(_manager._callbacks.count(entityID) == 0)
	{
		_manager._callbacks[entityID] = CollisionManager::CollisionArray();
		_manager._callbacks[entityID]._nb = 0;
	}

	_manager._callbacks[entityID]._callbaks[_manager._callbacks[entityID]._nb] = callback;
	++_manager._callbacks[entityID]._nb;
}