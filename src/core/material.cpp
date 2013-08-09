#include "core/material.h"
#include <memory>

void material::init(Material& mat, int* texData, int w, int h, int color)
{
	mat.color = color;
	mat.w = w;
	mat.h = h;

	if(texData != 0)
	{
		mat._texData = new int[w * h];
		memcpy(mat._texData, texData, w * h * sizeof(int));
	}
	else
	{
		mat._texData = 0;
	}
}

//draw the entire texData, if some, to the buffer.
void material::drawTo(Material& mat, Renderer& buffer, int x, int y)
{
	alfar::Rect src = {{0,0},{mat.w, mat.h}};
	alfar::Rect dst = {{x,y},{mat.w, mat.h}};

	drawTo(mat, buffer, src, dst);
}

//draw the entire image to destRect(so scaled if destRect < texData);
void material::drawTo(Material& mat, Renderer& buffer, const alfar::Rect& destRect)
{
	alfar::Rect src = {{0,0},{mat.w, mat.h}};

	drawTo(mat, buffer, src, destRect);
}

//draw srcRect to destRect(scaled to that size)
void material::drawTo(Material& mat, Renderer& buffer, const alfar::Rect& srcRect, const alfar::Rect& destRect)
{
	//clamped to buffer dim to avoid out of buffer writing
	alfar::Rect trueDest;

	trueDest.min.x = std::max(destRect.min.x, 0.0f);
	trueDest.min.y = std::max(destRect.min.y, 0.0f);
	trueDest.max.x = std::min(destRect.max.x, (float)buffer.w);
	trueDest.max.y = std::min(destRect.max.y, (float)buffer.h);

	int style = mat._texData == NULL ? 0 : 1 ;

	float wRatio = (srcRect.max.x - srcRect.min.x) / (destRect.max.x - destRect.min.x);
	float hRatio = (srcRect.max.y - srcRect.min.y) / (destRect.max.y - destRect.min.y);

	int srcW = srcRect.max.x - srcRect.min.x;
	int srcH = srcRect.max.y - srcRect.min.y;
	int destW = trueDest.max.x - trueDest.min.x;
	int destH = trueDest.max.y - trueDest.min.y;

	for(int y = 0; y < destH; ++y)
	{
		for(int x = 0; x < destW; ++x)
		{
			int dx = trueDest.min.x + x;
			int dy = trueDest.min.y + y;
			int sx = srcRect.min.x + (x/(float)destW) * srcW;
			int sy = srcRect.min.y + (y/(float)destH) * srcH;

			switch(style)
			{
			case 0:
				buffer.buffer[dy * buffer.w + dx] = mat.color;
				break;
			case 1:
				buffer.buffer[dy * buffer.w + dx] = mat._texData[sy * mat.w + sx];
				break;
			default:break;
			}
		}
	}
}

//--------------------------------------------------------------

void material::loadImg(Material& mat, int* data, int w, int h)
{
	mat.w = w;
	mat.h = h;
	mat._texData = new int[w*h];
	memcpy(mat._texData, data, h*w*sizeof(int));
}