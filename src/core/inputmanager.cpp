#include "core/inputmanager.h"

InputManager _manager;

void inputmanager::update()
{
	const uint8_t* keyStates = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		_manager._keyState[i].pressed = (keyStates[i] == 1);
	}

	int x,y;
	uint32_t buttons = SDL_GetMouseState(&x, &y);
	_manager._mousePos.x = x;
	_manager._mousePos.y = y;

	for(int i = 0; i < 3; ++i)
	{
		_manager._mouseButton[i].pressed = buttons & SDL_BUTTON(i) != 0;
	}
}

//===============================================

bool inputmanager::haveFocus(SDL_Window* win)
{
	return SDL_GetMouseFocus() == win;
}

alfar::Vector2 inputmanager::mousePos()
{
	return _manager._mousePos;
}

bool inputmanager::mousePressed(int button)
{
	return _manager._mouseButton[button].pressed;
}

bool inputmanager::keyPressed(uint32_t keycode)
{
	uint32_t key = SDL_GetScancodeFromKey(keycode);
	return _manager._keyState[key].pressed;
}