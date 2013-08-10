#include "SDL.h"

#include "core/renderer.h"
#include "mapinfo.h"
#include "core/material.h"
#include "core/inputmanager.h"
#include "core/entity.h"
#include "component/camera.h"
#include "editor/mapdisplay.h"

#include "helper/lodepng.h"


int main(int argc, char* argv[])
{
	foundation::memory_globals::init();

	MaterialManager::init();
	EntityManager::init();
	CameraManager::init();

	SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *writableTex = NULL;

    int posX = 100, posY = 100, width = 800, height = 600;

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	writableTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	Renderer rend;
	rend.w = width;
	rend.h = height;
	rend.buffer = new unsigned int[rend.h * rend.w];

	int matID = MaterialManager::createObject();

	Material& mat = MaterialManager::getObject(matID);
	material::init(mat);

	unsigned error;
	unsigned int* image;
	unsigned int w, h;

	error = lodepng_decode32_file((unsigned char**)&image, &w, &h, "data/originalWolf.png");
	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

	material::loadImg(mat, image, 0, 0, 64, 64, w, h, true);

	int matID2 = MaterialManager::createObject();

	Material& mat2 = MaterialManager::getObject(matID2);
	material::init(mat2);

	material::loadImg(mat2, image, 64, 0, 64, 64, w, h, true);

	free(image);

	mat.color = 0xFF000000;

	MapInfo map;
	LineInfo lI = {{-2,2}, {2,2}, 1, matID};
	mapinfo::addLine(map, lI);
	LineInfo lI2 = {{2,2}, {2,-2}, 1, matID2};
	mapinfo::addLine(map, lI2);

	editor::MapDisplay mapdisp;
	editor::mapdisplay::open(mapdisp, map);

	Entity& p = EntityManager::createAndGet();
	p.position.y = -3;

	Camera& c = camera::addToEntity(p);
	

    while (1) {
            SDL_Event e;
            if (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                            break;
                    }       
            }

			inputmanager::update();

			renderer::clearBuffer(rend, 0x00000000);
			renderer::raytraceMap(rend, map);

			Uint32* pixels;
			int pitch;
			SDL_LockTexture(writableTex, NULL, (void**)&pixels, &pitch);
			memcpy(pixels, rend.buffer, rend.w * rend.h * sizeof(int));
			SDL_UnlockTexture(writableTex);

			editor::mapdisplay::update(mapdisp, map);

            SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, writableTex, NULL, NULL);
            SDL_RenderPresent(renderer);
    }

	delete rend.buffer;
	SDL_DestroyTexture(writableTex);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    return 0;
}