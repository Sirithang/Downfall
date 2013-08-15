#include "component/behaviour.h"
#include "core/inputmanager.h"
#include "core/collisionmanager.h"
#include "mapinfo.h"
#include "SDL.h"

void created(Behaviour& b, int i)
{
	b._idx = i;
	b._entity = -1;

	
	b._luaState = lua_open();
	luaL_openlibs(b._luaState);

	behaviour::setupScript(b);
}

void behaviour::update()
{
	uint32_t nb = 0;
	Behaviour* bs = BehaviourManager::getDataPtr(nb);

	for(int i = 0; i < nb; ++i)
	{
		if(bs[i]._luaState == NULL)
			continue;

		lua_getglobal(bs[i]._luaState, "onUpdate");
		if (lua_pcall(bs[i]._luaState, 0, 0, 0) != 0)
		{
			printf("error running function");
			printf(lua_tostring(bs[i]._luaState, -1));
			lua_pop(bs[i]._luaState, 1);
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
			printf("error running function \n");
			printf("%s \n",lua_tostring(b._luaState, -1));
			lua_pop(b._luaState, 1);
		}

		strcpy_s(b.file, file);
	}
}

void behaviour::reloadAll()
{
	uint32_t size;
	Behaviour* bs = BehaviourManager::getDataPtr(size);

	for(int i = 0; i < size; ++i)
	{
		if(bs[i]._idx == -1)
			continue;

		behaviour::setScriptFile(bs[i], bs[i].file);
	}
}

void behaviour::attacheToEntity(Behaviour& b, Entity& e)
{
	ComponentInfo info;
	info._idx = b._idx;
	info._type = BEHAVIOUR;

	entity::addComponentInfo(e, info);
	b._entity = e._idx;

	Callback16 callback;
	callback.f = behaviour::onCollision;
	memcpy(callback.data, &b._idx, sizeof(b._idx));

	collisionmanager::registerCallback(b._entity, callback);
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
	{"getDirection", behaviour::functions::getDirection},
	{"getAngle", behaviour::functions::getAngle},
	{"setAngle", behaviour::functions::setAngle},
	{"sphereCollide", behaviour::functions::sphereCollide},
	{NULL, NULL}
};

 void behaviour::setupScript(Behaviour& bs)
{
	lua_State* L = bs._luaState;

	luaL_openlib(L, "DF", behaviourLib, 0);

	//---- Global key Setup 

	lua_newtable(L);
	for(int i = 0 ; i < 512; ++i)
	{
		const char* keyname = SDL_GetKeyName(i);
		if(strcmp(keyname, "") == 0)
			continue;

		lua_pushstring(L, keyname);
		lua_pushinteger(L, i);
		lua_settable(L, -3);
	}
	lua_setglobal(L, "KEYS");
	
	lua_pushlightuserdata(bs._luaState, (void*)bs._idx);
	lua_setglobal(L, "this");
}

 void behaviour::onCollision(void* data, unsigned int callerID, unsigned int collideeID)
 {

	 int idx;
	 memcpy(&idx, data, sizeof(int));

	 if(idx < 0)
		 return;

	 Behaviour& b = BehaviourManager::getObject(idx);

	lua_getglobal(b._luaState, "onCollision");
	lua_pushinteger(b._luaState, collideeID);
		
	if (lua_pcall(b._luaState, 1, 0, 0) != 0)
	{
		printf("error running function \n");
		printf("%s \n",lua_tostring(b._luaState, -1));
		lua_pop(b._luaState, 1);
	}
 }

 //=======================================================

 int behaviour::functions::isKeyPressed(lua_State* L)
{
	int code = lua_tointeger(L, -1);

	lua_pushboolean(L, inputmanager::keyPressed(code));

	return 1;
}

int behaviour::functions::setPosition(lua_State* L)
{
	int curr = (int)lua_topointer(L, 1);

	Behaviour& b = BehaviourManager::getObject(curr);

	if(b._entity < 0)
	{
		return 0;
	}

	Entity& ent = EntityManager::getObject(b._entity);

	float x = lua_tonumber(L, 2);
	float y = lua_tonumber(L, 3);

	ent.position.x = x;
	ent.position.y = y;

	return 0;
}

int behaviour::functions::getPosition(lua_State* L)
{
	int curr = (int)lua_topointer(L, 1);

	Behaviour& b = BehaviourManager::getObject(curr);

	if(b._entity < 0)
	{
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);

		return 2;
	}

	Entity& ent = EntityManager::getObject(b._entity);

	lua_pushnumber(L, ent.position.x);
	lua_pushnumber(L, ent.position.y);

	return 2;
}

int behaviour::functions::getDirection(lua_State* L)
{
	int curr = (int)lua_topointer(L, 1);

	Behaviour& b = BehaviourManager::getObject(curr);

	if(b._entity < 0)
	{
		lua_pushnumber(L, 0);
		lua_pushnumber(L, 0);

		return 2;
	}

	Entity& ent = EntityManager::getObject(b._entity);
	alfar::Vector2 dir = entity::getForward(ent);

	lua_pushnumber(L, dir.x);
	lua_pushnumber(L, dir.y);

	return 2;
}

int behaviour::functions::getAngle(lua_State* L)
{
	int curr = (int)lua_topointer(L, 1);

	Behaviour& b = BehaviourManager::getObject(curr);

	if(b._entity < 0)
	{
		lua_pushnumber(L, 0);
		return 1;
	}

	Entity& ent = EntityManager::getObject(b._entity);

	lua_pushnumber(L, ent.angle);

	return 1;
}

int behaviour::functions::setAngle(lua_State* L)
{
	int curr = (int)lua_topointer(L, 1);

	Behaviour& b = BehaviourManager::getObject(curr);

	if(b._entity < 0)
	{
		return 0;
	}

	Entity& ent = EntityManager::getObject(b._entity);

	float angle = lua_tonumber(L, 2);
	ent.angle = angle;

	return 1;
}

int behaviour::functions::sphereCollide(lua_State* L)
{
	float x = lua_tonumber(L, 1);
	float y = lua_tonumber(L, 2);
	float r = lua_tonumber(L, 3);

	alfar::Vector2 pos = {x,y};
	bool res = mapinfo::sphereCollide(*mapinfo::current, pos, r);

	lua_pushboolean(L, res);

	return 1;
}