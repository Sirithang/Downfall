#pragma once

#include "core/component.h"
#include "core/objectmanager.h"
#include "core/entity.h"
#include "core/renderer.h"

struct Sprite : public Component
{
	alfar::Rect uv;

	int _material;
};

typedef ObjectManager<Sprite, 32> Spritemanager;
void created(Sprite& spr, int i);
void destroyed(Sprite& spr);

namespace sprite
{
	void addToEntity(Sprite& spr, Entity& ent);

	void drawAll(Renderer& rend);
}