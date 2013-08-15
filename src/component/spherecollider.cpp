#include "component/spherecollider.h"
#include "math/vector2.h"

void created(SphereCollider& collider, int i)
{
	collider._idx = i;
	collider.radius = 0.5f;
	collider._position.x = 0;
	collider._position.y = 0;
}

void destroyed(SphereCollider& collider)
{
	collider._idx = -1;
}

//==============================================

foundation::Array<CollisionManager::Collision> spherecollider::testAgainstLines(SphereCollider* colliders, LineInfo* lines, int nbCollider, int nbLines)
{
	foundation::Array<CollisionManager::Collision> out(foundation::memory_globals::default_allocator());

	for(int i = 0; i < nbCollider; ++i)
	{
		SphereCollider& coll = colliders[i];
		spherecollider::syncWithEntity(coll);

		for(int j = 0; j < nbLines; ++j)
		{
			LineInfo& ln = lines[j];

			alfar::Vector2 startToCenter = coll._position - ln.start;
			alfar::Vector2 lineV = alfar::vector2::normalize(ln.end - ln.start);

			float dot = alfar::vector2::dot(startToCenter, lineV);

			alfar::Vector2 testPts = ln.start + lineV*dot;

			if(alfar::vector2::sqrMagnitude(coll._position - testPts) < coll.radius*coll.radius)
			{
				CollisionManager::Collision c;
				c._colliderID = coll._entity;
				c._collideeID = j;

				foundation::array::push_back(out, c);
			}
		}
	}

	return out;
}

//==================================================

void spherecollider::addToEntity(SphereCollider& collider, Entity& ent)
{
	ComponentInfo info;
	info._idx = collider._idx;
	info._type = ComponentType::SPHERECOLLIDER;

	entity::addComponentInfo(ent, info);

	collider._entity = ent._idx;
}

void spherecollider::syncWithEntity(SphereCollider& collider)
{
	if(collider._entity < 0)
		return;

	collider._position = EntityManager::getObject(collider._entity).position;
}