#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include <glm/glm.hpp>

#include "Scene.h"

class Camera;
class Shader;
class Texture;
class Terrain;
class Mesh;
class PhysicalObject;

class GameScene: public Scene
{
private:
	glm::mat4 model;
	glm::vec3 light_pos;

	float camX;
	float camY;
	Camera* main_camera;

	Shader* terrainShader;
	Shader* objectShader;

	Texture* terrainTexture;
	Texture* objectTexture;

	Terrain* terrain;
	Mesh* terrainMesh;

	PhysicalObject* object;
	Mesh* objectMesh;

	void HandleEvents(float delta);
	void HandleShaders();
public:
	GameScene(int width, int height);
	~GameScene();

	void Start();
	void Final();
};

#endif
