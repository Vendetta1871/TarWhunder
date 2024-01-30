#include "../../include/scene/GameScene.h"
#include "../../include/system/Window.h"
#include "../../include/system/Events.h"
#include "../../include/system/Camera.h"
#include "../../include/graphics/Mesh.h"
#include "../../include/graphics/RenderMesh.h"
#include "../../include/graphics/Texture.h"
#include "../../include/graphics/Shader.h"
#include "../../include/mathf/Collider.h"
#include "../../include/physics/PhysicalObject.h"
#include "../../include/physics/PhysicsProcessing.h"
#include "../../include/gameobject/Terrain.h"
#include "../../include/gameobject/TerrainGenerator.h"

#include <iostream>
#include <time.h>

#include <glm/ext.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GameScene::GameScene(int width, int height) : Scene(width, height)
{
	Window::Init(this->width, this->height, "GameWindow");
	Events::Init();

	model = glm::mat4(1.0f);
	lightPosition = glm::vec3(32.0f, 50.0f, 32.0f);

	camX = 0.0f;
	camY = 0.0f;
	mainCamera = new Camera(glm::vec3(1.0f, 1.0f, 1.0f), glm::radians(70.0f));

	terrainShader = LoadShader("shaders/gouradT.vert", "shaders/gouradT.fraq");
	objectsShader = LoadShader("shaders/gourad.vert", "shaders/gourad.fraq");
	debugShader = LoadShader("shaders/debug.vert", "shaders/debug.fraq");
	if (terrainShader == nullptr || objectsShader == nullptr || debugShader == nullptr)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		status = -1;
		return;
	}

	terrainTexture = LoadTexture("resources/textures/grass.png");
	objectsTexture = LoadTexture("resources/textures/cube.png");
	if (terrainTexture == nullptr || objectsTexture == nullptr)
	{
		std::cerr << "Failed to load texture" << std::endl;
		status = -1;
		return;
	}

	terrain = TerrainGenerator::GenerateHeightsMap(64);
	terrainMesh = RenderMesh::RenderTerrainMesh(terrain);

	for (int i = 0; i < 100; ++i)
	{
		objects.push_back(new PhysicalObject(glm::vec3(10, 5 + 7 * i, 10), 2, 2, 2, 1));
		objectsMesh.push_back(RenderMesh::RenderPhysicalObjectMesh(objects[i]));
	}

	PhysicsProcessing::InitPhysics(terrain, objects);

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

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPointSize(20.0f);
	glLineWidth(5.0f);
	glClearColor(117 / 255.0f, 187 / 255.0f, 253 / 255.0f, 1);
	// main loop
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
		lightPosition = lightPosition * rot;

		HandleEvents(delta);

		HandleShaders();

		Window::SwapBuffers();
		Events::PollEvents();

		if (!isPhysicsPaused)
		{
			Collider::Collisions.clear();
			PhysicsProcessing::SimulatePhysics(delta);
		}

		// fps counter
		if (time > 2.0f)
		{
			std::cout << frames / time << std::endl;
			frames = 1;
			time = 0;
		}
		else
		{
			frames += 1;
			time += delta;
		}


		// fps limit
		while (false && delta < 1.0f / 30)
		{
			currentTime = cps * clock();
			delta = currentTime - lastTime;
		}
	}

	delete terrainShader;
	delete objectsShader;
	delete terrainTexture;
	delete objectsTexture;

	delete terrain;
	delete terrainMesh;

	Window::Terminate();
}

void GameScene::HandleEvents(float delta)
{
	float speed = 5.0f;

	if (Events::JustKeyPressed(GLFW_KEY_ESCAPE))
	{
		Window::SetShouldClose(true);
	}
	if (Events::JustKeyPressed(GLFW_KEY_TAB))
	{
		Events::ToggleCursor();
	}
	if (Events::JustKeyPressed(GLFW_KEY_LEFT_ALT))
	{
		isDebug = !isDebug;
	}
	if (Events::JustKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		isPhysicsPaused = !isPhysicsPaused;
	}

	if (Events::IsKeyPressed(GLFW_KEY_W))
	{
		mainCamera->Position += mainCamera->Front * delta * speed;
	}
	if (Events::IsKeyPressed(GLFW_KEY_S))
	{
		mainCamera->Position -= mainCamera->Front * delta * speed;
	}
	if (Events::IsKeyPressed(GLFW_KEY_D))
	{
		mainCamera->Position += mainCamera->Right * delta * speed;
	}
	if (Events::IsKeyPressed(GLFW_KEY_A))
	{
		mainCamera->Position -= mainCamera->Right * delta * speed;
	}
	if (Events::IsKeyPressed(GLFW_KEY_SPACE))
	{
		mainCamera->Position += mainCamera->Up * delta * speed;
	}
	if (Events::IsKeyPressed(GLFW_KEY_LEFT_CONTROL))
	{
		mainCamera->Position -= mainCamera->Up * delta * speed;
	}

	if (Events::IsCursorLocked) 
	{
		camY += -Events::DeltaY / Window::Height * 2;
		camX += -Events::DeltaX / Window::Height * 2;

		camY = std::max(camY, -glm::radians(89.0f));
		camY = std::min(camY, glm::radians(89.0f));

		mainCamera->Rotation = glm::mat4(1.0f);
		mainCamera->Rotate(camY, camX, 0);
	}
}

void GameScene::HandleShaders()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// draw terrain
	terrainShader->Use();
	terrainShader->SetUniformMatrix("model", model);
	terrainShader->SetUniformMatrix("projview",
		mainCamera->GetProjection() * mainCamera->GetView());
	terrainShader->SetUniformVector("lightPos", lightPosition);
	terrainTexture->Bind();
	terrainMesh->Draw(GL_TRIANGLES);
	// draw objects
	objectsShader->Use();
	objectsShader->SetUniformMatrix("model", model);
	objectsShader->SetUniformMatrix("projview",
		mainCamera->GetProjection() * mainCamera->GetView());
	objectsShader->SetUniformVector("lightPos", lightPosition);
	objectsTexture->Bind();
	int count = std::min(objects.size(), objectsMesh.size());
	for (int i = 0; i < count; ++i)
	{
		objectsShader->SetUniformVector("move", objects[i]->Position);
		objectsShader->SetUniformMatrix("rotation", objects[i]->Rotation);
		objectsMesh[i]->Draw(GL_TRIANGLES);
	}
	// draw colliders bounds and collision points
	if (isDebug) {
		debugShader->Use();
		debugShader->SetUniformMatrix("model", model);
		debugShader->SetUniformMatrix("projview",
			mainCamera->GetProjection() * mainCamera->GetView());

		debugShader->SetUniformVector("clr", glm::vec3(0.0f, 1.0f, 0.0f));
		Mesh* debugMesh = RenderMesh::RenderDebugPhysicsMesh(objects);
		debugMesh->Draw(GL_LINES);
		delete debugMesh;

		debugShader->SetUniformVector("clr", glm::vec3(0.8f, 0.0f, 0.8f));
		debugMesh = RenderMesh::RenderDebugPhysicsMesh(Collider::Collisions);
		debugMesh->Draw(GL_POINTS);
		delete debugMesh;
	}
}
