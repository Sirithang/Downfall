#include "component/camera.h"
#include "core/entity.h"

int camera::mainCam = -1;

void created(Camera& cam, int i)
{
	cam.planeSize = 1.0f;

	if(camera::mainCam == -1)
		camera::mainCam = i;

	cam._idx = i;
}

void destroyed(Camera& cam)
{

}


Camera& camera::addToEntity(Entity& entity)
{
	int newCam = CameraManager::createObject();
	Camera& cam = CameraManager::getObject(newCam);

	cam._entity = entity._idx;

	ComponentInfo info;
	info._type = ComponentType::CAMERA;
	info._idx = newCam;

	entity::addComponentInfo(entity, info);

	return cam;
}