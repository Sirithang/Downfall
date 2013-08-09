#pragma once

#include "array.h"

template<class T, int DefaultSize>
class ObjectManager
{
protected:
	static ObjectManager* _instance;

	foundation::Array<T> _objects;
	foundation::Array<int> _freeIDs;
	int upperID;

	ObjectManager();

public:
	static void init();

	static int createObject();
	static void freeObject(int i);
	static T& getObject(int i);
};

//=======================================

template<class T, int DefaultSize>
ObjectManager<T, DefaultSize>* ObjectManager<T, DefaultSize>::_instance = NULL;

template<class T, int DefaultSize>
ObjectManager<T, DefaultSize>::ObjectManager() 
	:_objects(foundation::memory_globals::default_allocator()),
	_freeIDs(foundation::memory_globals::default_allocator()),
	upperID(0)
{
	foundation::array::reserve(_objects, DefaultSize);
	foundation::array::reserve(_freeIDs, 64);
}

//****

template<class T, int DefaultSize>
void ObjectManager<T, DefaultSize>::init()
{
	_instance = new ObjectManager<T, DefaultSize>();
}

template<class T, int DefaultSize>
int ObjectManager<T, DefaultSize>::createObject()
{
	int created = -1;
	if(foundation::array::size(_instance->_freeIDs) > 0)
	{
		created = foundation::array::back(_instance->_freeIDs);
		foundation::array::pop_back(_instance->_freeIDs);
	}
	else
	{
		created = _instance->upperID;
		++_instance->upperID;
	}

	return created;
}

//------------------------------------

template<class T, int DefaultSize>
void ObjectManager<T, DefaultSize>::freeObject(int i)
{
	foundation::array::push_back(_instance->_freeIDs, i);
}

//----------------------------------

template<class T, int DefaultSize>
T& ObjectManager<T, DefaultSize>::getObject(int i)
{
	return _instance->_objects[i];
}