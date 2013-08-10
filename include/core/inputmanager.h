#pragma once

#include <SDL.h>

struct KeyState
{
	bool pressed;
};

struct InputManager
{
	KeyState _keyState[SDL_NUM_SCANCODES];
};

namespace inputmanager
{
	void update();
	bool keyPressed(uint32_t keyCode);
}