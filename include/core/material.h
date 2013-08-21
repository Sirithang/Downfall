#pragma once

#include "math/math_types.h"
#include "core/renderer.h"
#include "core/objectmanager.h"

union RGBA
{
	struct{unsigned char A,B,G,R;};
	uint32_t integer;
};

struct Material
{
	//color in 32bit RGBA 0xRRGGBBAA
	int color;
	//NULL if no tex.
	unsigned int* _texData;
	int w,h;
};

namespace material
{
	void init(Material& mat, int* texData = 0, int w = 0, int h = 0, int color = 0xFFFFFFFF);

	/// if flip is true, it reverse the bit order of each pixel (RGBA to ABGR) usefull ofr endianess.
	void loadImg(Material& mat, unsigned int* data, int srcW, int srcH, const bool flip = false);

	//draw the entire texData, if some, to the buffer.
	void drawTo(Material& mat, Renderer& buffer, int x, int y);
	//draw the entire image to destRect(so scaled if destRect < texData);
	void drawTo(Material& mat, Renderer& buffer, const alfar::Rect& destRect);
	//draw srcRect to destRect(scaled to that size)
	void drawTo(Material& mat, Renderer& buffer, const alfar::Rect& srcRect, const alfar::Rect& destRect);
}

void created(Material& mat, int i);
void destroyed(Material& mat);

typedef ObjectManager<Material, 256> MaterialManager;