#pragma once

#include "map_types.h"


struct RenderCommand
{
	//dist to viewer, those rendercommand are sorted back to front.
	float dist;
	alfar::Rect source;
	alfar::Rect dest;
	uint32_t material;
};

struct Renderer
{
	unsigned int* buffer;
	int w,h;
	float* zbuffer;

	RenderCommand _command[4096];
	int _nbCommand;
};


namespace renderer
{
	void create(Renderer& rend, int w, int h);

	void clearBuffer(Renderer& rend, int value);
	void raytraceMap(Renderer& renderer, const MapInfo& map);

	void insertCommand(Renderer& rend, RenderCommand command);
	void executeRenderCommand(Renderer& rend);
}

//-----------------

namespace rendercommand
{
	int backToFrontCmp(const void* A, const void* B);
}