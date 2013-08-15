#include "component/sprite.h"
#include "component/camera.h"

void created(Sprite& spr, int i)
{
	spr._idx = i;
	spr._material = 0;
}

void destroyed(Sprite& spr)
{
	spr._idx = -1;
}

void sprite::drawAll(Renderer& rend)
{
	Camera& cam = CameraManager::getObject(camera::mainCam);
	Entity& play = EntityManager::getObject(cam._entity);

	uint32_t nb;
	Sprite* sprs = Spritemanager::getDataPtr(nb);

	for(int i = 0; i < nb; ++i)
	{

	}
}