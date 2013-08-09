#include "core/renderer.h"
#include "math/vector2.h"
#include "mapinfo.h"
#include "array.h"
#include "core/material.h"
#include <math.h>

using namespace renderer;


void renderer::clearBuffer(Renderer& rend, int value)
{
	memset(rend.buffer, value, rend.w * rend.h * sizeof(int));
}

//========================================

float angle = 0.0f;

void renderer::raytraceMap(Renderer& rend, const MapInfo& map)
{
	//todo : debug remove
	alfar::Vector2 camPos = {0,0};
	alfar::Vector2 lookDir = {0,1};

	angle += 0.005f;
	lookDir = alfar::vector2::normalize(alfar::vector2::rotate(lookDir, angle));

	alfar::Vector2 perpDir = {lookDir.y,-lookDir.x};

	for(int x = 0; x < rend.w; ++x)
	{
		float ratio = (x - (rend.w*0.5f)) / (rend.w*0.5f);

		alfar::Vector2 dir = alfar::vector2::normalize(camPos + lookDir + alfar::vector2::mul(perpDir, ratio));

		alfar::Ray2D ray;
		ray.start = camPos;
		ray.direction = dir;
		ray.dist = 1000.0f;

		RayIntersect inter = lineinfo::rayCheck(ray, map._lines._data, foundation::array::size(map._lines));

		if(inter.lineID >= 0)
		{
			alfar::Vector2 camToPts = ray.start + ray.direction * inter.distance;
			alfar::Vector2 forwardProj = camPos + (lookDir * alfar::vector2::dot(camToPts, lookDir));
			float dist = alfar::vector2::magnitude(forwardProj);

			LineInfo ln = map._lines[inter.lineID];
			Material& mat = MaterialManager::getObject(ln.material);

			int h = 1.0f / dist * rend.h;
			h = std::min(h, rend.h);

			int y = (rend.h / 2 - h/2);

			alfar::Rect src = {{inter.purcentage * mat.w,0},{1,mat.h}};
			alfar::Rect dst = {{x,y},{x+1,y+h}};

			/*for(int l = 0; l < h; ++l)
			{
				int y = (rend.h / 2 - h/2) + l;

				rend.buffer[y * rend.w + x] = 0x00FF0000;
			}*/

			material::drawTo(mat, rend, src, dst);
		}
	}
}