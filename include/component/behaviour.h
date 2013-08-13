#pragma once

#include "core/component.h"
#include "core/objectmanager.h"

extern "C"
{
	#include <lua.h>
	#include <lauxlib.h>
	#include <lualib.h>
}

struct Behaviour : public Component
{
	lua_State* _luaState;
};

typedef ObjectManager<Behaviour, 128> BehaviourManager;
void created(Behaviour& b, int i);
void destroyed(Behaviour& b);

namespace behaviour
{
	void setScriptFile(Behaviour& p_Script, const char* p_File);

	void update();

	namespace functions
	{
		int isKeyPressed(lua_State* L);
		int setPosition(lua_State* L);
		int getPosition(lua_State* L);
	}
}