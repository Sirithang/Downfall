#pragma once

#include <SDL.h>
#include "math/math_types.h"

struct KeyState
{
	bool pressed;
};

struct MouseState
{
	bool pressed;
};

struct InputManager
{
	KeyState _keyState[SDL_NUM_SCANCODES];

	MouseState _mouseButton[3];
	alfar::Vector2 _mousePos;
};

namespace inputmanager
{
	void update();
	bool keyPressed(uint32_t keyCode);
	alfar::Vector2 mousePos();
	bool mousePressed(int button);
	bool haveFocus(SDL_Window* win);
}