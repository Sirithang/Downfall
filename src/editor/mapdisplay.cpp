#include "editor/mapdisplay.h"
#include "core/entity.h"
#include "math/vector2.h"
#include "component/camera.h"

#define DISPLAY_W 640
#define DISPLAY_H 480

void editor::mapdisplay::open(MapDisplay& disp, MapInfo info)
{
	disp._win = SDL_CreateWindow("Map Display", 10, 10, DISPLAY_W, DISPLAY_H, 0);
	 disp._map = info;
	 disp._renderer = SDL_CreateRenderer(disp._win, -1, SDL_RENDERER_ACCELERATED);;
	 draw(disp);
}

void editor::mapdisplay::update(MapDisplay& disp, MapInfo info)
{
	disp._map = info;
	draw(disp);
}

void editor::mapdisplay::draw(MapDisplay& disp)
{
	int idCam = camera::mainCam;
	if(idCam < 0)
		return;

	Camera& cam = CameraManager::getObject(idCam);
	Entity&  player = EntityManager::getObject(cam._entity);

	alfar::Vector2 forward =  alfar::vector2::normalize(entity::getForward(player));
	alfar::Vector2 perp = {forward.y, -forward.x};
	perp = alfar::vector2::normalize(perp);

	SDL_SetRenderDrawColor(disp._renderer, 0,0,0,255);
	SDL_RenderClear(disp._renderer);

	SDL_Rect rect;
	rect.x = DISPLAY_W / 2;
	rect.y = DISPLAY_H / 2;
	rect.w = DISPLAY_W;
	rect.h = DISPLAY_H;


	float scale = 50;


	//DrawPlayer
	SDL_SetRenderDrawColor(disp._renderer, 0, 255, 0, 255);

	SDL_Point pts[4];
	pts[0].x = (player.position.x + forward.x * 0.4f) * scale + rect.x;
	pts[0].y = (player.position.y + forward.y * 0.4f) * scale+ rect.y;

	pts[1].x = (player.position.x - perp.x * 0.2f) * scale + rect.x;
	pts[1].y = (player.position.y - perp.y * 0.2f) * scale + rect.y;

	pts[2].x = (player.position.x + perp.x * 0.2f) * scale + rect.x;
	pts[2].y = (player.position.y + perp.y * 0.2f) * scale + rect.y;

	pts[3].x = pts[0].x;
	pts[3].y = pts[0].y;

	SDL_RenderDrawLines(disp._renderer, pts, 4);

	//Draw frustrum
	SDL_SetRenderDrawColor(disp._renderer, 0, 0, 255, 255);
	alfar::Vector2 dir = alfar::vector2::normalize(forward + (perp * -1));
	SDL_RenderDrawLine(disp._renderer, 
			(player.position.x) * scale + rect.x, 
			(player.position.y)* scale + rect.y, 
			(player.position.x + dir.x * 200.0f)* scale + rect.x, 
			(player.position.y + dir.y * 200.0f)* scale + rect.y);

	dir = alfar::vector2::normalize(forward + (perp * 1));
	SDL_RenderDrawLine(disp._renderer, 
			(player.position.x) * scale + rect.x, 
			(player.position.y)* scale + rect.y, 
			(player.position.x + dir.x * 200.0f)* scale + rect.x, 
			(player.position.y + dir.y * 200.0f)* scale + rect.y);

	// Draw Level


	SDL_SetRenderDrawColor(disp._renderer, 255, 0, 0, 255);

	for(int i = 0; i < foundation::array::size(disp._map._lines); ++i)
	{
		LineInfo& info = disp._map._lines[i];
		SDL_RenderDrawLine(disp._renderer, 
			(info.start.x - player.position.x) * scale + rect.x, 
			(info.start.y - player.position.y)* scale + rect.y, 
			(info.end.x - player.position.x)* scale + rect.x, 
			(info.end.y - player.position.y)* scale + rect.y);
	}

	SDL_RenderPresent(disp._renderer);
}