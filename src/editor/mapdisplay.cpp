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

	disp._currentSelection._currentIndex = -1;

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

	//draw grid
	SDL_SetRenderDrawColor(disp._renderer, 64, 64, 64, 255);

	//center clamped to the closest integer value
	int worldCenterX = center.x;
	int worldCenterY = center.y;

	for(int i = -10; i < 10; ++i)
	{
		SDL_RenderDrawLine(disp._renderer,	(worldCenterX + i - center.x) * scale.x + rect.x, 
											(worldCenterY + 10 - center.y) * scale.y + rect.y, 
											(worldCenterX + i - center.x) * scale.x + rect.x, 
											(worldCenterY - 10 - center.y) * scale.y + rect.y);
	}

	for(int j = -10; j < 10; ++j)
	{
		SDL_RenderDrawLine(disp._renderer,	(worldCenterX + 10 - center.x) * scale.x + rect.x, 
											(worldCenterY + j - center.y) * scale.y + rect.y, 
											(worldCenterX - 10 - center.x) * scale.x + rect.x, 
											(worldCenterY + j - center.y) * scale.y + rect.y);
	}

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

		if(info._idx == -1)
			continue;

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

		if(disp._currentSelection._currentType == Selection::VERTEX && disp._currentSelection._currentIndex == i)
		{
			SDL_SetRenderDrawColor(disp._renderer, 255, 0, 255, 255);
		}
		else
		{
			SDL_SetRenderDrawColor(disp._renderer, 128, 255, 0, 255);
		}

		int s = 0.2f * scale.x;

		SDL_Rect r;
		r.x = (vert[i]._position.x - center.x) * scale.x - s*0.5f + rect.x;
		r.y = (vert[i]._position.y - center.y) * scale.y - s*0.5f + rect.y;
		r.w = s;
		r.h = s;

		SDL_RenderDrawRect(disp._renderer, &r);
	}

	//-- draw entity

	Entity* ents = EntityManager::getDataPtr(size);
	for(int i = 0; i < size; ++i)
	{
		if(player._idx == i)
			continue;

		SDL_SetRenderDrawColor(disp._renderer, 128, 128, 255, 255);

		int s = 0.2f * scale.x;

		SDL_Rect r;
		r.x = (ents[i].position.x - center.x) * scale.x - s*0.5f + rect.x;
		r.y = (ents[i].position.y - center.y) * scale.y - s*0.5f + rect.y;
		r.w = s;
		r.h = s;

		SDL_RenderDrawRect(disp._renderer, &r);
	}

	SDL_RenderPresent(disp._renderer);
}

//==========================================

//control info
alfar::Vector2 clickedPos;

enum Mode{NONE, VERT_MOVE, VERT_CREATE, CAM_MOV};
Mode currentMode = NONE;

void editor::mapdisplay::handleInput(MapDisplay& disp, MapInfo& info)
{
	if(!inputmanager::haveFocus(disp._win))
		return;

	int idCam = camera::mainCam;
	if(idCam < 0)
		return;

	
	alfar::Vector2 mouse = inputmanager::mousePos();
	Camera& cam = CameraManager::getObject(idCam);
	Entity&  player = EntityManager::getObject(cam._entity);

	alfar::Vector2 worldPos;
	worldPos.x = (mouse.x - DISPLAY_W/2.0f) * (1.0f/scale.x) + center.x;
	worldPos.y = (mouse.y - DISPLAY_H/2.0f) * (1.0f/scale.y) + center.y;

	uint32_t size;
	MapVertex* vert = VertexManager::getDataPtr(size);

	if(inputmanager::mousePressed(MouseState::LEFT))
	{
		if(currentMode == NONE)
		{
			if(inputmanager::keyPressed(SDLK_LCTRL))
			{
				currentMode = VERT_CREATE;

				if(disp._currentSelection._currentType == Selection::VERTEX && disp._currentSelection._currentIndex != -1)
				{
					int closest = mapvertex::getClosest(worldPos);

					if(closest == -1)
					{
						MapVertex& v = mapvertex::add(worldPos);
						closest = v._index;
					}

					mapvertex::createLine(VertexManager::getObject(disp._currentSelection._currentIndex), vert[closest], info);

					disp._currentSelection._currentType = Selection::VERTEX;
					disp._currentSelection._currentIndex = closest;
				}
				else
				{
					MapVertex& v = mapvertex::add(worldPos);
					disp._currentSelection._currentType = Selection::VERTEX;
					disp._currentSelection._currentIndex = v._index;
				}
			}
			else if(inputmanager::keyPressed(SDLK_LSHIFT))
			{
				int close = mapvertex::getClosest(worldPos);
				if(disp._currentSelection._currentType == Selection::VERTEX && disp._currentSelection._currentIndex != -1 && close != -1)
				{
					MapVertex& v = vert[close];
					MapVertex& v2 = vert[disp._currentSelection._currentIndex];

					bool create = true;
					for(int i = 0; i < v._nbLines; ++i)
					{
						if(v._linkedLines[i].idx == -1)
							continue;

						for(int j = 0; j < v2._nbLines; ++j)
						{
							if(v2._linkedLines[j].idx == -1)
								continue;

							if(v._linkedLines[i].idx == v2._linkedLines[j].idx)
								create = false;
						}
					}

					if(create)
					{
						mapvertex::createLine(v2, v, info);
					}
				}
			}
			else
			{
				// -- check for which vertex is selected;
				currentMode = VERT_MOVE;
				disp._currentSelection._currentType = Selection::VERTEX;
				disp._currentSelection._currentIndex = mapvertex::getClosest(worldPos);
			}
			clickedPos = mouse;
		}
	}
	else if(inputmanager::mousePressed(MouseState::RIGHT))
	{
		if(currentMode == NONE)
		{
			currentMode = CAM_MOV;
			clickedPos = mouse;
		}
	}
	else
	{
		currentMode = NONE;
	}

	scale = scale + alfar::vector2::create(3, -3) * inputmanager::mouseWheel();
	scale.x = scale.x < 0 ? 0 : scale.x;
	scale.y = scale.y > 0 ? 0 : scale.y;

	if(currentMode == VERT_MOVE)
	{
		if(disp._currentSelection._currentType == Selection::VERTEX && disp._currentSelection._currentIndex != -1)
		{
			alfar::Vector2 p = worldPos;
			if(inputmanager::keyPressed(SDLK_LALT))
			{
				p.x = std::floor(p.x);
				p.y = std::floor(p.y);
			}

			mapvertex::move(vert[disp._currentSelection._currentIndex], info, p);
		}
	}
	else if(currentMode == CAM_MOV)
	{
		alfar::Vector2 delta = mouse - clickedPos;
		clickedPos = mouse;

		center.x -= delta.x * (1.0f/scale.x);
		center.y -= delta.y * (1.0f/scale.y);
	}

	//====keyboard

	if(inputmanager::keyPressed(SDLK_DELETE))
	{
		if(disp._currentSelection._currentType == Selection::VERTEX && disp._currentSelection._currentIndex != -1)
		{
			mapinfo::removeVertex(info, disp._currentSelection._currentIndex);
			disp._currentSelection._currentIndex = -1;
		}
	}
}