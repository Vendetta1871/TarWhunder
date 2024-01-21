#include "Scene.h"
#include "GameScene.h"

#include <glm/glm.hpp>
#include <iostream>

int main() 
{
	Scene* scene = new GameScene(1080, 720);
	scene->Start();
	scene->Final();
	delete scene;
}
