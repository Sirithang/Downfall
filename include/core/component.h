#pragma once

enum ComponentType
{
	CAMERA,
	BEHAVIOUR
};

struct Component
{
	int _idx;
	int _entity;
};

struct ComponentInfo
{
	int _idx;
	ComponentType _type;
};