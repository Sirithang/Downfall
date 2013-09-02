#pragma once

#include "map_types.h"
#include "SDL.h"

namespace editor
{
	struct Selection
	{
		enum SelectedType{VERTEX, ENTITY};

		SelectedType _currentType;
		int _currentIndex;
	};

	struct MapDisplay
	{
		SDL_Window* _win;
		SDL_Renderer* _renderer;
		MapInfo _map;
		
		Selection _currentSelection;
	};

	namespace mapdisplay
	{
		void open(MapDisplay& disp, MapInfo& info);
		void update(MapDisplay& disp, MapInfo& info);
		void draw(MapDisplay& disp);

		void drawSelectedInspector(MapDisplay& disp);

		void handleInput(MapDisplay& disp, MapInfo& info);
	}
}