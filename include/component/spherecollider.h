#pragma once

#include "core/component.h"
#include "core/objectmanager.h"
#include "math/math_types.h"

#include "core/collisionmanager.h"
#include "array.h"
#include "core/entity.h"

struct SphereCollider : public Component
{
	alfar::Vector2 _position;
	float radius;
	CollisionCallback _callback;
};

typedef ObjectManager<SphereCollider, 64> SphereColliderManager;
void created(SphereCollider& collider, int i);
void destroyed(SphereCollider& collider);

namespace spherecollider
{
	void addToEntity(SphereCollider& collider, Entity& ent);
	void syncWithEntity(SphereCollider& collider);

	foundation::Array<CollisionManager::Collision> testAgainstLines(SphereCollider* colliders, LineInfo* lines, int nbCollider, int nbLines);
}