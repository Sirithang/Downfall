#include "SDL.h"

#include "core/renderer.h"
#include "mapinfo.h"

int main(int argc, char* argv[])
{
	foundation::memory_globals::init();

	SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *writableTex = NULL;

    int posX = 100, posY = 100, width = 320, height = 240;

    win = SDL_CreateWindow("Hello World", posX, posY, width, height, 0);

    renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

	writableTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

	Renderer rend;
	rend.w = 320;
	rend.h = 240;
	rend.buffer = new int[rend.h * rend.w];

	MapInfo map;
	const LineInfo lI = {{-2,2}, {2,2}, 1};
	mapinfo::addLine(map, lI);

    while (1) {
            SDL_Event e;
            if (SDL_PollEvent(&e)) {
                    if (e.type == SDL_QUIT) {
                            break;
                    }       
            }

			renderer::clearBuffer(rend, 0x00000000);
			renderer::raytraceMap(rend, map);

			Uint32* pixels;
			int pitch;
			SDL_LockTexture(writableTex, NULL, (void**)&pixels, &pitch);
			memcpy(pixels, rend.buffer, rend.w * rend.h * sizeof(int));
			SDL_UnlockTexture(writableTex);

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