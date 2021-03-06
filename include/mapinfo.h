#pragma once

#include "map_types.h"

void created(MapVertex& vert, int i);
void destroyed(MapVertex& vert);

namespace mapvertex
{
	MapVertex& add(alfar::Vector2 position);

	void move(MapVertex& vert, MapInfo& info, alfar::Vector2 newPos);

	LineInfo& createLine(MapVertex& start, MapVertex& end, MapInfo& map);
	void removeLine(MapVertex& vert, LineInfo& line);

	void addLineLink(MapVertex& vertex, const LineInfo& line, char side);

	//grab the closest vertex index from that position in worldspace. return -1 if no closer than "limit"
	int getClosest(alfar::Vector2 position, float limit = 0.5f);
}

//----------------------------

namespace lineinfo
{
	float length(LineInfo& line);
	RayIntersect rayCheck(const alfar::Ray2D inRay, const LineInfo* inLines, int number = 1);
}

//----------------------------

namespace mapinfo
{
	extern MapInfo* current;

	LineInfo& addLine(MapInfo& map, const LineInfo& info);
	void removeLine(MapInfo& map, int id);
	void removeVertex(MapInfo& map, int id);

	bool sphereCollide(MapInfo& map, alfar::Vector2 pos, float radius);
}