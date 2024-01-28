#include "GameScene.h"

#include <iostream>
#include <time.h>

#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "system/Window.h"
#include "system/Events.h"
#include "system/Camera.h"

#include "graphics/Mesh.h"
#include "graphics/RenderMesh.h"
#include "graphics/Texture.h"
#include "graphics/Shader.h"

#include "Terrain.h"
#include "TerrainGenerator.h"

#include "physics/PhysicalObject.h"
#include "physics/PhysicsProcessing.h"
#include "physics/Collider.h"

GameScene::GameScene(int width, int height) : Scene(width, height)
{
	Window::Init(this->width, this->height, "GameWindow");
	Events::Init();

	model = glm::mat4(1.0f);
	light_pos = glm::vec3(32.0f, 50.0f, 32.0f);

	camX = 0.0f;
	camY = 0.0f;
	main_camera = new Camera(glm::vec3(1, 1, 1), glm::radians(70.0f));

	terrainShader = LoadShader("shaders/gouradT.vert", "shaders/gouradT.fraq");
	objectShader = LoadShader("shaders/gourad.vert", "shaders/gourad.fraq");
	debugShader = LoadShader("shaders/debug.vert", "shaders/debug.fraq");
	if (terrainShader == nullptr || objectShader == nullptr || debugShader == nullptr)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		status = -1;
		return;
	}

	terrainTexture = LoadTexture("resources/grass.png");
	objectTexture = LoadTexture("resources/cube.png");
	if (terrainTexture == nullptr || objectTexture == nullptr)
	{
		std::cerr << "Failed to load texture" << std::endl;
		status = -1;
		return;
	}

	terrain = TerrainGenerator::CreateTerrain();
	terrainMesh = RenderMesh::RenderTerrainMesh(terrain);

	for (int i = 0; i < 10; ++i) 
	{
		object.push_back(new PhysicalObject(glm::vec3(10, 5 + 7 * i, 10 + 0.8f * i), 2, 2, 2, 1));
		objectMesh.push_back(RenderMesh::RenderPhysicalObjectMesh(object[i]));
	}

	int n = 0;
	float** map = terrain->GetHeightMap(&n);
	PhysicsProcessing::InitPhysics(map, n, &object);

	isDebug = false;
	isPhysicsPaused = false;
}

GameScene::~GameScene()
{
	Window::Terminate();
}

float cps = 1.0f / CLOCKS_PER_SEC;

void GameScene::Start() 
{
	float lastTime = (float)glfwGetTime();
	float currentTime = 0.0f;
	float delta = 0.0f;
	// fps counter
	int frames = 0;
	float time = 0;
	//
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(20.0f);
	glLineWidth(5.0f);

	glClearColor(117 / 255.0f, 187 / 255.0f, 253 / 255.0f, 1);

	while (!Window::GetShouldClose())
	{
		currentTime = cps * clock();
		delta = currentTime - lastTime;
		lastTime = currentTime;
		
		// light rotation
		float angle = glm::radians(delta * 100);
		glm::mat3x3 rot = glm::mat3x3(
			glm::cos(angle), 0, glm::sin(angle),
			0, 1, 0,
			-glm::sin(angle), 0, glm::cos(angle));
		light_pos = light_pos * rot;
		// 

		HandleEvents(delta);

		HandleShaders();

		Window::SwapBuffers();
		Events::PollEvents();

		if (!isPhysicsPaused)
		{
			Collider::Collisions.clear();
			PhysicsProcessing::ComputePhysics(delta);
		}

		//* FPS COUNTER
		if (++frames > 1000)
		{
			std::cout << frames / time << std::endl;
			frames = 0;
			time = 0;
		}
		else
		{
			time += delta;
		}
		//*/

		//* FPS LIMIT
		while (false && delta < 1.0f / 30)
		{
			currentTime = cps * clock();
			delta = currentTime - lastTime;
		}
		//*/
	}

	delete terrainShader;
	delete objectShader;
	delete terrainTexture;
	delete objectTexture;

	delete terrain;
	delete terrainMesh;

	Window::Terminate();
}

void GameScene::HandleEvents(float delta)
{
	float speed = 5.0f;

	if (Events::JustPressed(GLFW_KEY_ESCAPE))
	{
		Window::SetShouldClose(true);
	}
	if (Events::JustPressed(GLFW_KEY_TAB))
	{
		Events::ToogleCursor();
	}
	if (Events::JustPressed(GLFW_KEY_LEFT_ALT))
	{
		isDebug = !isDebug;
	}
	if (Events::JustPressed(GLFW_KEY_LEFT_SHIFT))
	{
		isPhysicsPaused = !isPhysicsPaused;
	}

	if (Events::IsPressed(GLFW_KEY_W))
	{
		main_camera->Position += main_camera->Front * delta * speed;
	}
	if (Events::IsPressed(GLFW_KEY_S))
	{
		main_camera->Position -= main_camera->Front * delta * speed;
	}
	if (Events::IsPressed(GLFW_KEY_D))
	{
		main_camera->Position += main_camera->Right * delta * speed;
	}
	if (Events::IsPressed(GLFW_KEY_A))
	{
		main_camera->Position -= main_camera->Right * delta * speed;
	}
	if (Events::IsPressed(GLFW_KEY_SPACE))
	{
		main_camera->Position += main_camera->Up * delta * speed;
	}
	if (Events::IsPressed(GLFW_KEY_LEFT_CONTROL))
	{
		main_camera->Position -= main_camera->Up * delta * speed;
	}

	if (Events::IsCursorLocked) 
	{
		camY += -Events::DeltaY / Window::Height * 2;
		camX += -Events::DeltaX / Window::Height * 2;

		if (camY < -glm::radians(89.0f)) 
		{
			camY = -glm::radians(89.0f);
		}
		if (camY > glm::radians(89.0f)) 
		{
			camY = glm::radians(89.0f);
		}

		main_camera->Rotation = glm::mat4(1.0f);
		main_camera->Rotate(camY, camX, 0);
	}
}

void GameScene::HandleShaders()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	terrainShader->Use();
	terrainShader->UniformMatrix("model", model);
	terrainShader->UniformMatrix("projview",
		main_camera->GetProjection() * main_camera->GetView());
	terrainShader->UniformVector("lightPos", light_pos);
	terrainTexture->Bind();
	terrainMesh->Draw(GL_TRIANGLES);


	objectShader->Use();
	objectShader->UniformMatrix("model", model);
	objectShader->UniformMatrix("projview",
		main_camera->GetProjection() * main_camera->GetView());
	objectShader->UniformVector("lightPos", light_pos);
	objectTexture->Bind();
	for (int i = 0; i < 10; ++i)
	{
		objectShader->UniformVector("move", object[i]->r);
		objectShader->UniformMatrix("rotation", object[i]->A);
		objectMesh[i]->Draw(GL_TRIANGLES);
	}

	if (isDebug) {
		debugShader->Use();
		debugShader->UniformMatrix("model", model);
		debugShader->UniformMatrix("projview",
			main_camera->GetProjection() * main_camera->GetView());

		debugShader->UniformVector("clr", glm::vec3(0.0f, 1.0f, 0.0f));
		Mesh* debugMesh = RenderMesh::RenderDebugPhysicsMesh(PhysicsProcessing::object);
		debugMesh->Draw(GL_LINES);
		delete debugMesh;

		debugShader->UniformVector("clr", glm::vec3(0.8f, 0.0f, 0.8f));
		debugMesh = RenderMesh::RenderDebugPhysicsMesh(Collider::Collisions);
		debugMesh->Draw(GL_POINTS);
		delete debugMesh;
	}
}

void GameScene::Final() 
{

}
