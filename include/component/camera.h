#pragma once

#include "core/component.h"
#include "math/math_types.h"
#include "core/objectmanager.h"

struct Entity;

struct Camera : public Component
{
	float planeSize;
};

typedef ObjectManager<Camera, 16> CameraManager;
void created(Camera& cam, int i);
void destroyed(Camera& cam);

namespace camera
{
	extern int mainCam;

	Camera& addToEntity(Entity& entity);
}