#include "mapinfo.h"
#include "math/vector2.h"
#include "array.h"

RayIntersect lineinfo::rayCheck(const alfar::Ray2D inRay, const LineInfo* inLines, int number)
{
	RayIntersect ret;
	ret.distance = inRay.dist * inRay.dist;
	ret.lineID = -1;

	alfar::Vector2 rayPerp = alfar::vector2::create(inRay.direction.y, -inRay.direction.x);

	for(int i = 0; i < number; ++i)
	{
		LineInfo line = inLines[i];

		alfar::Vector2 lineV = alfar::vector2::sub(line.end, line.start);
		alfar::Vector2 rayToLine = alfar::vector2::sub(line.start, inRay.start);

		if(alfar::vector2::dot(lineV, inRay.direction) < 0.000001f)
			continue; // the two vector are coolinear or nearly.

		float s = (rayPerp.y * rayToLine.x - rayPerp.x * rayToLine.y) / (rayPerp.x * lineV.y - rayPerp.y * lineV.x);

		if(s < 0.0f || s > 1.0f)
			continue;

		alfar::Vector2 pts = alfar::vector2::add(line.start, alfar::vector2::mul(lineV, s));

		float d = alfar::vector2::sqrMagnitude(alfar::vector2::sub(pts, inRay.start));

		if(d < ret.distance)
		{
			ret.lineID = i;
			ret.distance = d;
			ret.point = pts;
		}
	}

	return ret;
}


//============================================================================

using namespace mapinfo;

MapInfo::MapInfo()
	: _lines(foundation::memory_globals::default_allocator())
{

}

void mapinfo::addLine(MapInfo& map, const LineInfo& info)
{
	foundation::array::push_back(map._lines, info);
}