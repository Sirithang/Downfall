#include "core/material.h"
#include "helper/helpers.h"
#include <memory>

void material::init(Material& mat, int* texData, int w, int h, int color)
{
	mat.color = color;
	mat.w = w;
	mat.h = h;

	if(texData != 0)
	{
		mat._texData = new unsigned int[w * h];
		memcpy(mat._texData, texData, w * h * sizeof(unsigned int));
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

	trueDest.min.x = destRect.min.x;//std::max(destRect.min.x, 0.0f);
	trueDest.min.y = destRect.min.y;//std::max(destRect.min.y, 0.0f);
	trueDest.max.x = destRect.max.x;//std::min(destRect.max.x, (float)buffer.w);
	trueDest.max.y = destRect.max.y;//std::min(destRect.max.y, (float)buffer.h);

	int style = mat._texData == NULL ? 0 : 1 ;

	float wRatio = (srcRect.max.x - srcRect.min.x) / (destRect.max.x - destRect.min.x);
	float hRatio = (srcRect.max.y - srcRect.min.y) / (destRect.max.y - destRect.min.y);

	int srcW = srcRect.max.x - srcRect.min.x;
	int srcH = srcRect.max.y - srcRect.min.y;
	int destW = trueDest.max.x - trueDest.min.x;
	int destH = trueDest.max.y - trueDest.min.y;

	for(int y = 0; y < destH; ++y)
	{
		int dy = trueDest.min.y + y;
		int sy = srcRect.min.y + (y/(float)destH) * srcH;
		if(dy < 0 || dy >= buffer.h)
				continue;

		for(int x = 0; x < destW; ++x)
		{
			int dx = trueDest.min.x + x;
			int sx = srcRect.min.x + (x/(float)destW) * srcW;

			if(dx < 0 || dx >= buffer.w)
				continue;

			RGBA Src;
			RGBA Tex;

			Src.integer = buffer.buffer[dy * buffer.w + dx];
			Tex.integer = mat._texData[sy * mat.w + sx];

			float alpha = Tex.A / 255.0f;
			float invAlpha = 1.0f - alpha;

			RGBA Result;
			Result.R = Src.R * invAlpha + Tex.R * alpha;
			Result.G = Src.G * invAlpha + Tex.G * alpha;
			Result.B = Src.B * invAlpha + Tex.B * alpha;
			Result.A = Src.A * invAlpha + Tex.A * alpha;

			switch(style)
			{
			case 0:
				buffer.buffer[dy * buffer.w + dx] = mat.color;
				break;
			case 1:
				buffer.buffer[dy * buffer.w + dx] = Result.integer;
				break;
			default:break;
			}
		}
	}
}

//--------------------------------------------------------------

void material::loadImg(Material& mat, unsigned int* data, int srcW, int srcH, const bool flip)
{
	mat.w = srcW;
	mat.h = srcH;
	mat._texData = new unsigned int[srcW*srcH];

	for(int j = 0; j < srcH; ++j)
	{
		for(int i = 0; i < srcW; ++i)
		{
			mat._texData[j * srcW + i] = reverseBytes(data[j * srcW + i]);
		}
	}
}

//=================================================

void created(Material& mat, int i)
{
	material::init(mat);
}

void destroyed(Material& mat)
{

}