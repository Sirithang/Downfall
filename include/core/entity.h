#pragma once

#include "array.h"
#include "math/math_types.h"
#include "core/component.h"
#include "core/ObjectManager.h"

struct Entity
{
	ComponentInfo _component[128];
	int _nbComp;

	int _idx;

	alfar::Vector2 position;
	float angle;
};

typedef ObjectManager<Entity, 256> EntityManager;
void created(Entity& ent, int i);
void destroyed(Entity& ent);

//------------------------

namespace entity
{
	void addComponentInfo(Entity& entity, ComponentInfo info);
	ComponentInfo getComponentInfo(Entity& entity, ComponentType type);

	alfar::Vector2 getForward(Entity& entity);

}