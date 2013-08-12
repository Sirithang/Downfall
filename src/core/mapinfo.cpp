#include "mapinfo.h"
#include "math/vector2.h"
#include "array.h"

void created(MapVertex& vert, int i)
{
	vert._index = i;
	vert._nbLines = 0;
	vert._position.x = 0;
	vert._position.y = 0;

	for(int i = 0; i < 128; ++i)
	{
		vert._linkedLines[i].idx = -1;
	}
}

MapVertex& mapvertex::add(alfar::Vector2 pos)
{
	MapVertex& vert = VertexManager::createAndGet();

	vert._position = pos;

	return vert;
}

void mapvertex::move(MapVertex& vert, MapInfo& info, alfar::Vector2 pos)
{
	vert._position = pos;

	for(int i = 0; i < vert._nbLines; ++i)
	{
		if(vert._linkedLines[i].idx == -1)
			continue;

		if(vert._linkedLines[i].side == 0)
		{
			info._lines[vert._linkedLines[i].idx].start = vert._position = pos;
		}
		else
		{
			info._lines[vert._linkedLines[i].idx].end = vert._position = pos;
		}
	}
}

LineInfo& mapvertex::createLine(MapVertex& start, MapVertex& end, MapInfo& map)
{
	LineInfo line;

	line.start = start._position;
	line.startVertex = start._index;

	line.end = end._position;
	line.endVertex = end._index;

	line.material = 0;

	line = mapinfo::addLine(map, line);

	mapvertex::addLineLink(start, line, 0);
	mapvertex::addLineLink(end, line, 1);

	return line;
}

void mapvertex::removeLine(MapVertex& vert, LineInfo& line)
{
	for(int i = 0; i < vert._nbLines; ++i)
	{
		if(vert._linkedLines[i].idx == line._idx)
		{
			vert._linkedLines[i].idx = -1;
			if(i == vert._nbLines -1)
				vert._nbLines -= 1;

			return;
		}
	}
}

void mapvertex::addLineLink(MapVertex& vertex, const LineInfo& line, char side)
{
	int i = 0;
	while(vertex._linkedLines[i].idx != -1)
		++i;

	vertex._linkedLines[i].idx = line._idx;
	vertex._linkedLines[i].side = side;
	vertex._nbLines = i + 1;
}


//=========================================================================================

RayIntersect lineinfo::rayCheck(const alfar::Ray2D inRay, const LineInfo* inLines, int number)
{
	RayIntersect ret;
	ret.distance = inRay.dist * inRay.dist;
	ret.lineID = -1;

	alfar::Vector2 rayPerp = alfar::vector2::create(inRay.direction.y, -inRay.direction.x);

	for(int i = 0; i < number; ++i)
	{
		LineInfo line = inLines[i];

		alfar::Vector2 lineV = line.end - line.start;
		alfar::Vector2 rayToLine = line.start - inRay.start;

		if(std::abs(alfar::vector2::dot(lineV, inRay.direction)) < 0.000001f)
			continue; // the two vector are coolinear or nearly.

		float uv = (inRay.direction.x * lineV.y - inRay.direction.y * lineV.x);
		float s = (inRay.direction.y * rayToLine.x - inRay.direction.x * rayToLine.y) / uv;

		if(s < 0.0f || s > 1.0f)
			continue;

		float t = (lineV.y * rayToLine.x - lineV.x * rayToLine.y) / uv;

		if(t < 0)
			continue;

		alfar::Vector2 pts = alfar::vector2::add(line.start, alfar::vector2::mul(lineV, s));

		float d = alfar::vector2::sqrMagnitude(pts - inRay.start);

		if(d < ret.distance)
		{
			ret.lineID = i;
			ret.purcentage = s;
			ret.distance = d;
			ret.point = pts;
		}
	}

	return ret;
}

//------

float lineinfo::length(LineInfo& line)
{
	return alfar::vector2::magnitude(line.end - line.start);
}


//============================================================================

using namespace mapinfo;

MapInfo::MapInfo()
	: _lines(foundation::memory_globals::default_allocator()),
	_freeLinesIds(foundation::memory_globals::default_allocator())
{

}

LineInfo& mapinfo::addLine(MapInfo& map, const LineInfo& info)
{
	if(foundation::array::size(map._freeLinesIds) > 0)
	{
		int id = foundation::array::back(map._freeLinesIds);
		foundation::array::pop_back(map._freeLinesIds);

		map._lines[id] = info;
		map._lines[id]._idx = id;

		return map._lines[id];
	}
	else
	{
		foundation::array::push_back(map._lines, info);
		LineInfo& info = foundation::array::back(map._lines);
		info._idx = foundation::array::size(map._lines) - 1;

		return info;
	}
}

void mapinfo::removeLine(MapInfo& map, int id)
{
	mapvertex::removeLine(VertexManager::getObject(map._lines[id].startVertex), map._lines[id]);
	mapvertex::removeLine(VertexManager::getObject(map._lines[id].endVertex), map._lines[id]);

	map._lines[id]._idx = -1;
	foundation::array::push_back(map._freeLinesIds, id);
}