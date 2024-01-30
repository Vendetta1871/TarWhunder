#include "../../include/scene/Scene.h"

Scene::Scene(int width, int height) : width(width), height(height)
{
	if (width < 1 || height << 1)
	{
		this->width = 1080;
		this->height = 720;
	}
	status = 0;
}

Scene::~Scene()
{
	
}
