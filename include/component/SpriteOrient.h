#pragma once

#include "core/component.h"
#include "core/entity.h"
#include "core/objectmanager.h"

struct SpriteOrient : public Component
{
	int nbAngles;
};

typedef ObjectManager<SpriteOrient, 128> SpriteOrientManager;
void created(SpriteOrient& sprOr, int idx);
void destroyed(SpriteOrient& sprOr);

namespace spriteorient
{
	void addToEntity(SpriteOrient& sprOr, Entity& ent);

	void updateAll();
}