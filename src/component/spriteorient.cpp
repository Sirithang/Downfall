#include "component/spriteorient.h"
#include "component/camera.h"
#include "component/sprite.h"

#include "math/vector2.h"

void created(SpriteOrient& sprOr, int idx)
{
	sprOr._idx = idx;
	sprOr.nbAngles = 1;
}

void destroyed(SpriteOrient& sprOr)
{
	sprOr._idx = -1;
}

//------------------------------------------------

void spriteorient::addToEntity(SpriteOrient& sprOr, Entity& ent)
{
	sprOr._entity = ent._idx;
	
	ComponentInfo info;
	info._idx = sprOr._idx;
	info._type = SPRITEORIENT;

	entity::addComponentInfo(ent, info);
}

void spriteorient::updateAll()
{
	Camera cam = CameraManager::getObject(camera::mainCam);
	Entity player = EntityManager::getObject(cam._entity);

	alfar::Vector2 forward = entity::getForward(player);
	alfar::Vector2 left = alfar::vector2::create(forward.y, -forward.x);

	uint32_t size;
	SpriteOrient* sprOrs = SpriteOrientManager::getDataPtr(size);

	for(int i = 0; i < size; ++i)
	{
		if(sprOrs[i]._idx < 0)
			continue;

		Entity ent = EntityManager::getObject(sprOrs[i]._entity);

		ComponentInfo sprInf = entity::getComponentInfo(ent, SPRITE);

		if(sprInf._idx == -1)
			continue;

		Sprite& spr = Spritemanager::getObject(sprInf._idx);

		alfar::Vector2 sprForward = entity::getForward(ent);

		float dot = alfar::vector2::dot(sprForward, forward);
		float dotLeft = alfar::vector2::dot(sprForward, left);
		
		dot = (dot + 1.0f) * 0.25f;

		if(dotLeft < 0)
			dot = 1.0f - dot;

		int angle = (sprOrs[i].nbAngles * dot) + 0.5f;

		spr.uv.min.y = spr.uv.max.y * angle;
	}
}