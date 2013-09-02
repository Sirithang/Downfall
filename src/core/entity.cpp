#include "core/entity.h"
#include "math/vector2.h"

//=======================================


void created(Entity& ent, int i)
{
	ent._idx = i;

	ent._nbComp = 0;

	ent.position.x = 0;
	ent.position.y = 0;

	ent.angle = 0;
}

void destroyed(Entity& ent)
{
	
}

//=====================================

void entity::addComponentInfo(Entity& entity, ComponentInfo info)
{
	entity._component[entity._nbComp] = info;
	entity._nbComp += 1;
}

ComponentInfo entity::getComponentInfo(Entity& ent, ComponentType type)
{
	for(int i = 0 ; i < ent._nbComp; ++i)
	{
		if(ent._component[i]._type == type)
			return ent._component[i];
	}

	ComponentInfo inf;
	inf._idx = -1;
	return inf;
}


alfar::Vector2 entity::getForward(Entity& entity)
{
	alfar::Vector2 vec = {0, 1};
	return alfar::vector2::rotate(vec, entity.angle);
}