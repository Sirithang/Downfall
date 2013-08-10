#pragma once

#include "map_types.h"
#include "SDL.h"

namespace editor
{
	struct MapDisplay
	{
		SDL_Window* _win;
		SDL_Renderer* _renderer;
		MapInfo _map;
	};

	namespace mapdisplay
	{
		void open(MapDisplay& disp, MapInfo info);
		void update(MapDisplay& disp, MapInfo info);
		void draw(MapDisplay& disp);
	}
}