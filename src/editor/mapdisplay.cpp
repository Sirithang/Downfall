#include "editor/mapdisplay.h"
#include "core/entity.h"
#include "core/inputmanager.h"
#include "mapinfo.h"
#include "math/vector2.h"
#include "component/camera.h"

#define DISPLAY_W 640
#define DISPLAY_H 480

alfar::Vector2 scale;
alfar::Vector2 center;

void editor::mapdisplay::open(MapDisplay& disp, MapInfo& info)
{
	disp._win = SDL_CreateWindow("Map Display", 10, 10, DISPLAY_W, DISPLAY_H, 0);
	disp._map = info;
	disp._renderer = SDL_CreateRenderer(disp._win, -1, SDL_RENDERER_ACCELERATED);

	disp._vertSelected = -1;

	scale = alfar::vector2::create(50, -50);
	center = alfar::vector2::create(0,0);

	draw(disp);
}

void editor::mapdisplay::update(MapDisplay& disp, MapInfo& info)
{
	handleInput(disp, info);

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


	


	//DrawPlayer
	SDL_SetRenderDrawColor(disp._renderer, 0, 255, 0, 255);

	SDL_Point pts[4];
	pts[0].x = ((player.position.x + forward.x * 0.4f) - center.x) * scale.x + rect.x;
	pts[0].y = ((player.position.y + forward.y * 0.4f) - center.y) * scale.y + rect.y;

	pts[1].x = ((player.position.x - perp.x * 0.2f) - center.x) * scale.x + rect.x;
	pts[1].y = ((player.position.y - perp.y * 0.2f) - center.y) * scale.y + rect.y;

	pts[2].x = ((player.position.x + perp.x * 0.2f) - center.x) * scale.x + rect.x;
	pts[2].y = ((player.position.y + perp.y * 0.2f) - center.y) * scale.y + rect.y;

	pts[3].x = pts[0].x;
	pts[3].y = pts[0].y;

	SDL_RenderDrawLines(disp._renderer, pts, 4);

	//Draw frustrum
	SDL_SetRenderDrawColor(disp._renderer, 0, 0, 255, 255);
	alfar::Vector2 dir = alfar::vector2::normalize(forward + (perp * -1));
	SDL_RenderDrawLine(disp._renderer, 
			(player.position.x - center.x ) * scale.x + rect.x, 
			(player.position.y - center.y ) * scale.y + rect.y, 
			((player.position.x + dir.x * 200.0f) - center.x ) * scale.x + rect.x, 
			((player.position.y + dir.y * 200.0f) - center.y ) * scale.y + rect.y);

	dir = alfar::vector2::normalize(forward + (perp * 1));
	SDL_RenderDrawLine(disp._renderer, 
			(player.position.x - center.x ) * scale.x + rect.x, 
			(player.position.y - center.y ) * scale.y + rect.y, 
			((player.position.x + dir.x * 200.0f) - center.x ) * scale.x + rect.x, 
			((player.position.y + dir.y * 200.0f) - center.y ) * scale.y + rect.y);

	// Draw Level


	SDL_SetRenderDrawColor(disp._renderer, 255, 0, 0, 255);

	for(int i = 0; i < foundation::array::size(disp._map._lines); ++i)
	{
		LineInfo& info = disp._map._lines[i];
		SDL_RenderDrawLine(disp._renderer, 
			(info.start.x - center.x) * scale.x + rect.x, 
			(info.start.y - center.y)* scale.y + rect.y, 
			(info.end.x - center.x)* scale.x + rect.x, 
			(info.end.y - center.y)* scale.y + rect.y);
	}

	//-- Draw vertices

	uint32_t size;
	MapVertex* vert = VertexManager::getDataPtr(size);
	for(int i = 0; i < size; ++i)
	{
		if(vert[i]._index == -1)
			continue;

		if(disp._vertSelected == i)
		{
			SDL_SetRenderDrawColor(disp._renderer, 255, 0, 255, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(disp._renderer, 128, 255, 0, 255);
		}

		SDL_Rect r;
		r.x = (vert[i]._position.x - center.x) * scale.x - 5 + rect.x;
		r.y = (vert[i]._position.y - center.y) * scale.y - 5 + rect.y;
		r.w = 10;
		r.h = 10;

		SDL_RenderDrawRect(disp._renderer, &r);
	}

	SDL_RenderPresent(disp._renderer);
}

//==========================================

alfar::Vector2 clickedPos;

void editor::mapdisplay::handleInput(MapDisplay& disp, MapInfo& info)
{
	if(!inputmanager::haveFocus(disp._win))
		return;

	int idCam = camera::mainCam;
	if(idCam < 0)
		return;

	if(!inputmanager::mousePressed(0))
		return;

	Camera& cam = CameraManager::getObject(idCam);
	Entity&  player = EntityManager::getObject(cam._entity);

	alfar::Vector2 mouse = inputmanager::mousePos();

	alfar::Vector2 worldPos;
	worldPos.x = (mouse.x - DISPLAY_W/2.0f) * (1.0f/scale.x) + center.x;
	worldPos.y = (mouse.y - DISPLAY_H/2.0f) * (1.0f/scale.y) + center.y;

	uint32_t size;
	MapVertex* vert = VertexManager::getDataPtr(size);
	for(int i = 0; i < size; ++i)
	{
		if(vert[i]._index == -1)
			continue;

		if(std::abs(vert[i]._position.x - worldPos.x) < 0.5f && std::abs(vert[i]._position.y - worldPos.y) < 0.5f)
		{
			disp._vertSelected = i;
			break;
		}
	}

	if(disp._vertSelected != -1)
	{
		mapvertex::move(vert[disp._vertSelected], info, worldPos);
	}
}