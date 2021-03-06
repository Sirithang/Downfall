#pragma once

enum ComponentType
{
	CAMERA,
	BEHAVIOUR,
	SPHERECOLLIDER,
	SPRITE,
	SPRITEORIENT
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