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

void renderer::create(Renderer& rend, int w, int h)
{
	rend.w = w;
	rend.h = h;
	rend.buffer = new unsigned int[rend.h * rend.w];
	rend.zbuffer = new float[rend.w];
	
	rend._nbCommand = 0;
}

void renderer::clearBuffer(Renderer& rend, int value)
{
	memset(rend.buffer, value, rend.w * rend.h * sizeof(int));
	memset(rend.zbuffer, 0xffffffff, rend.w * sizeof(float));
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

			rend.zbuffer[x] = dist;

			LineInfo ln = map._lines[inter.lineID];
			Material& mat = MaterialManager::getObject(ln.material);

			int h = std::abs(rend.h / dist);

			int y = (rend.h / 2 - h/2);

			float wrappedUV = inter.purcentage * lineinfo::length(ln);
			wrappedUV = wrappedUV - std::floor(wrappedUV);

			RenderCommand cmd;

			alfar::Rect src = {{ln._uv.min.x + wrappedUV * ln._uv.max.x, ln._uv.min.y},{1,ln._uv.min.y+ln._uv.max.y}};
			alfar::Rect dst = {{x,y},{x+1,y+h}};

			cmd.source = src;
			cmd.dest = dst;
			cmd.dist = dist;
			cmd.material = ln.material;

			renderer::insertCommand(rend, cmd);

			//material::drawTo(mat, rend, src, dst);
		}
	}
}

void renderer::insertCommand(Renderer& rend, RenderCommand command)
{
	rend._command[rend._nbCommand] = command;
	rend._nbCommand += 1;
}

void renderer::executeRenderCommand(Renderer& rend)
{
	std::qsort(rend._command, rend._nbCommand, sizeof(RenderCommand), rendercommand::backToFrontCmp);

	for(int i = 0; i < rend._nbCommand; ++i)
	{
		RenderCommand& cmd = rend._command[i];
		Material& mat = MaterialManager::getObject(cmd.material);
		material::drawTo(mat, rend, cmd.source, cmd.dest);
	}

	rend._nbCommand = 0;
}


//========================================================

int rendercommand::backToFrontCmp(const void* A, const void* B)
{
	return ((const RenderCommand*)B)->dist - ((const RenderCommand*)A)->dist;
}