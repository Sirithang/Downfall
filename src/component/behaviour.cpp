#include "component/behaviour.h"

void created(Behaviour& b, int i)
{
	b._idx = i;
	b._luaState = lua_open();
	luaL_openlibs(b._luaState);
}

void behaviour::update()
{
	uint32_t nb = 0;
	Behaviour* bs = BehaviourManager::getDataPtr(nb);

	for(int i = 0; i < nb; ++i)
	{
		lua_getglobal(bs[i]._luaState, "onUpdate");
		if (lua_pcall(bs[i]._luaState, 0, 0, 0) != 0)
		{
			
		}
	}
}

void behaviour::setScriptFile(Behaviour& b, const char* file)
{
	int error = 0;

	error = luaL_loadfile(b._luaState, file);

	if(error != 0)
	{
		printf_s(lua_tostring(b._luaState, -1));
		lua_pop(b._luaState, 1); 
	}
	else
	{
		lua_pcall(b._luaState, 0,0,0);

		lua_getglobal(b._luaState, "onCreate");
		
		if (lua_pcall(b._luaState, 0, 0, 0) != 0)
		{
			printf("error running function");
			printf(lua_tostring(b._luaState, -1));
			lua_pop(b._luaState, 1);
		}
	}
}

//------

 void pushKeyInterger(lua_State *L, const char* key, int value)
 {
	lua_pushstring(L, key);
	lua_pushinteger(L, value);
	lua_settable(L, -3);
 }

 //================================

 void createMetatable(lua_State* L, const char* p_Name, const luaL_Reg* p_Lib)
{
	luaL_newmetatable(L, p_Name);
    
    lua_pushstring(L, "__index");
    lua_pushvalue(L, -2);  /* pushes the metatable */
    lua_settable(L, -3);  /* metatable.__index = metatable */
    
	luaL_openlib(L, NULL, p_Lib, 0);
}

 //==============================

 //**Entity Lib (interface also with transform)
static const struct luaL_reg behaviourLib [] = 
{
	{"setPosition", behaviour::functions::setPosition},
	{"getPosition", behaviour::functions::getPosition},
	{"isKeyPressed", behaviour::functions::isKeyPressed},
	{NULL, NULL}
};

 void setupScript(Behaviour& bs)
{
	lua_State* L = bs._luaState;

	luaL_openlib(bs._luaState, "DF", behaviourLib, 0);
	

	lua_pushlightuserdata(bs._luaState, (void*)bs._idx);
	lua_setglobal(L, "this");
}


 //=======================================================

 int behaviour::functions::isKeyPressed(lua_State* L)
{
	return 0;
}

int behaviour::functions::setPosition(lua_State* L)
{
	return 0;
}

int behaviour::functions::getPosition(lua_State* L)
{
	return 0;
}