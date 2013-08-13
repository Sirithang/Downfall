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
		_manager._collisions[_manager._nbCollisions] = infos[i];
		_manager._nbCollisions += 1;
	}
}

void collisionmanager::doCallbacks()
{
	for(int i = 0; i < _manager._nbCollisions; ++i)
	{
		CollisionManager::Collision& coll = _manager._collisions[i];
		CollisionCallback c = (CollisionCallback)coll._callback.f;
		c(coll._callback.data, coll._collideeID);
	}
}

void collisionmanager::reinit()
{
	_manager._nbCollisions = 0;
}