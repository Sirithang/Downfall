#include "component/sprite.h"
#include "component/camera.h"

#include "core/material.h"
#include "math/vector2.h"

void created(Sprite& spr, int i)
{
	spr._idx = i;
	spr._material = 0;

	alfar::Rect defaultUV = {{0,0},{0,0}};
	spr.uv = defaultUV;
}

void destroyed(Sprite& spr)
{
	spr._idx = -1;
}

void sprite::addToEntity(Sprite& spr, Entity& ent)
{
	spr._entity = ent._idx;
}

void sprite::drawAll(Renderer& rend)
{
	Camera& cam = CameraManager::getObject(camera::mainCam);
	Entity& play = EntityManager::getObject(cam._entity);

	alfar::Vector2 forward = entity::getForward(play);

	uint32_t nb;
	Sprite* sprs = Spritemanager::getDataPtr(nb);

	for(int i = 0; i < nb; ++i)
	{
		Entity& ent = EntityManager::getObject(sprs[i]._entity);
		alfar::Vector2 camToSpr = ent.position - play.position;

		float projOnForward = alfar::vector2::dot(camToSpr, forward);

		if(projOnForward < 0.0f)
			continue;

		float projOnSide = alfar::vector2::dot(camToSpr, alfar::vector2::create(forward.y / projOnForward, -forward.x / projOnForward));

		if(projOnSide < -1.5 || projOnSide > 1.5) // out of view
			continue;

		Material mat = MaterialManager::getObject(sprs[i]._idx);

		float ratio = sprs[i].uv.max.x / sprs[i].uv.max.y;

		int h = std::abs(rend.h / projOnForward);
		int w = h * ratio;

		int x = rend.w * ((projOnSide + 1.0f) * 0.5f) - w/2;
		int y = (rend.h / 2 - h/2);

		RenderCommand cmd;

		alfar::Rect src = { {sprs[i].uv.min.x , sprs[i].uv.min.y},
							{sprs[i].uv.min.x + sprs[i].uv.max.x , sprs[i].uv.min.y + sprs[i].uv.max.y}};

		alfar::Rect dst = {{x,y},{x+w,y+h}};

		cmd.source = src;
		cmd.dest = dst;
		cmd.dist = projOnForward;
		cmd.material = sprs[i]._material;

		renderer::insertCommand(rend, cmd);
	}
}