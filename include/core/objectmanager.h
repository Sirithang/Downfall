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
	static inline T& getObject(int i);

	//if you use this function, avoid stocking the return vaue, stock it's index instead (you need to have stocked it in the object in the created func)
	//(this can cause leak if you loose the object & its index)
	static T& createAndGet();

	static T* getDataPtr(uint32_t& size)
	{
		size = foundation::array::size(_instance->_objects);
		return _instance->_objects._data;
	}
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
	int id = -1;
	if(foundation::array::size(_instance->_freeIDs) > 0)
	{
		id = foundation::array::back(_instance->_freeIDs);
		foundation::array::pop_back(_instance->_freeIDs);
	}
	else
	{
		id = _instance->upperID;
		foundation::array::push_back(_instance->_objects, T());
		++_instance->upperID;
	}

	created(_instance->_objects[id], id);

	return id;
}

//------------------------------------

template<class T, int DefaultSize>
void ObjectManager<T, DefaultSize>::freeObject(int i)
{
	foundation::array::push_back(_instance->_freeIDs, i);
	destroyed(_instance->_objects[i]);
}

//----------------------------------

template<class T, int DefaultSize>
T& ObjectManager<T, DefaultSize>::getObject(int i)
{
	return _instance->_objects[i];
}

//---------------------------

template<class T, int DefaultSize>
T& ObjectManager<T,DefaultSize>::createAndGet()
{
	int obj = createObject();
	return getObject(obj);
}

//---------------------------

template<class T>
void destroyed(T& obj)
{
	//if you fail to compile here, mean you have an object using an ObjectManager that don't define it destroyed/created functions
	int data[0];
}

template<class T>
void created(T& obj, int i)
{
	//if you fail to compile here, mean you have an object using an ObjectManager that don't define it destroyed/created functions
	int data[0];
}