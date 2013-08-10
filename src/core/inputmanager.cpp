#include "core/inputmanager.h"

InputManager _manager;

void inputmanager::update()
{
	const uint8_t* keyStates = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < SDL_NUM_SCANCODES; ++i)
	{
		_manager._keyState[i].pressed = (keyStates[i] == 1);
	}
}

//===============================================

bool inputmanager::keyPressed(uint32_t keycode)
{
	uint32_t key = SDL_GetScancodeFromKey(keycode);
	return _manager._keyState[key].pressed;
}