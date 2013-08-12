#pragma once

#include <SDL.h>
#include "math/math_types.h"

struct KeyState
{
	bool pressed;
};

struct MouseState
{
	enum MouseButton
	{
		LEFT = SDL_BUTTON_LEFT ,
		MIDDLE = SDL_BUTTON_MIDDLE,
		RIGHT = SDL_BUTTON_RIGHT,
		X1 = SDL_BUTTON_X1,
		X2 = SDL_BUTTON_X2,
		MAX
	};
	bool pressed;
};

struct InputManager
{
	KeyState _keyState[SDL_NUM_SCANCODES];

	MouseState _mouseButton[MouseState::MAX];
	int _wheel;
	alfar::Vector2 _mousePos;
};

namespace inputmanager
{
	void update(int wheel = 0);
	bool keyPressed(uint32_t keyCode);
	
	alfar::Vector2 mousePos();
	bool mousePressed(MouseState::MouseButton button);
	int mouseWheel();

	bool haveFocus(SDL_Window* win);
}