#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "Scene.h"

#include <vector>

#include <glm/glm.hpp>

class Camera;
class Shader;
class Texture;
class Terrain;
class Mesh;
class PhysicalObject;
/*====================
3D scene of game world 
====================*/
class GameScene: public Scene
{
private:
	glm::mat4 model;

	float camX; // camera X axis rotation
	float camY; // camera Y axis rotation
	Camera* mainCamera;

	glm::vec3 lightPosition; // position of diffused light source

	Terrain* terrain;
	Mesh* terrainMesh;
	Shader* terrainShader;
	Texture* terrainTexture; // terrain texture atlas

	std::vector<PhysicalObject*> objects;
	std::vector<Mesh*> objectsMesh;
	Shader* objectsShader;
	Texture* objectsTexture; // object texture atlas
	
	//TODO: add debug class info
	Shader* debugShader;

	void HandleEvents(float delta);
	void HandleShaders();
public:
	bool isDebug;
	bool isPhysicsPaused;

	GameScene(int width, int height);
	~GameScene();

	void Start();
};

#endif
