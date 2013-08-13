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
		for(int j = 0; j < nbLines; ++j)
		{
			LineInfo& ln = lines[j];

			alfar::Vector2 startToCenter = coll._position - ln.start;
			alfar::Vector2 lineV = ln.end - ln.start;

			float dot = alfar::vector2::dot(startToCenter, lineV);

			alfar::Vector2 testPts = ln.start + lineV*dot;

			if(alfar::vector2::sqrMagnitude(testPts) < coll.radius*coll.radius)
			{
				CollisionManager::Collision c;
				c._colliderID = i;
				c._collideeID = j;
				c._callback.f = coll._callback;

				foundation::array::push_back(out, c);
			}
		}
	}

	return out;
}