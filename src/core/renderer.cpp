#include "core/renderer.h"
#include "math/vector2.h"
#include "mapinfo.h"
#include "array.h"
#include "core/material.h"
#include "core/inputmanager.h"
#include "core/entity.h"
#include "component/camera.h"
#include <math.h>

using namespace renderer;


void renderer::clearBuffer(Renderer& rend, int value)
{
	memset(rend.buffer, value, rend.w * rend.h * sizeof(int));
}

//========================================

void renderer::raytraceMap(Renderer& rend, const MapInfo& map)
{
	int camID = camera::mainCam;

	if(camID == -1)
		return;

	Camera& cam = CameraManager::getObject(camID);
	Entity& player = EntityManager::getObject(cam._entity);

	alfar::Vector2 lookDir = entity::getForward(player);

	if(inputmanager::keyPressed(SDLK_z))
	{
		player.position = player.position + (lookDir * 0.1f);
	}
	else if(inputmanager::keyPressed(SDLK_s))
	{
		player.position = player.position - (lookDir * 0.1f);
	}
	
	if(inputmanager::keyPressed(SDLK_d))
	{
		player.angle += -0.05f;
	}
	else if(inputmanager::keyPressed(SDLK_q))
	{
		player.angle += 0.05f;
	}

	alfar::Vector2 perpDir = {lookDir.y,-lookDir.x};
	perpDir = perpDir * cam.planeSize;

	for(int x = 0; x < rend.w; ++x)
	{
		float ratio = (x - (rend.w*0.5f)) / (rend.w*0.5f);

		alfar::Vector2 dir = alfar::vector2::normalize(lookDir + (perpDir * ratio));

		alfar::Ray2D ray;
		ray.start = player.position;
		ray.direction = dir;
		ray.dist = 1000.0f;

		RayIntersect inter = lineinfo::rayCheck(ray, map._lines._data, foundation::array::size(map._lines));

		if(inter.lineID >= 0)
		{
			alfar::Vector2 camToPts = inter.point - player.position;
			float dist = alfar::vector2::dot(camToPts, lookDir);

			if(dist < 0.1f)
				continue;

			LineInfo ln = map._lines[inter.lineID];
			Material& mat = MaterialManager::getObject(ln.material);

			int h = std::abs(rend.h / dist);

			int y = (rend.h / 2 - h/2);

			float wrappedUV = inter.purcentage * lineinfo::length(ln);
			wrappedUV = wrappedUV - std::floor(wrappedUV);

			alfar::Rect src = {{wrappedUV * mat.w,0},{1,mat.h}};
			alfar::Rect dst = {{x,y},{x+1,y+h}};

			material::drawTo(mat, rend, src, dst);
		}
	}
}