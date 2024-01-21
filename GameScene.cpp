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

GameScene::GameScene(int width, int height) : Scene(width, height)
{
	Window::Init(this->width, this->height, "GameWindow");
	Events::Init();

	model = glm::mat4(1.0f);
	light_pos = glm::vec3(32.0f, 50.0f, 32.0f);

	camX = 0.0f;
	camY = 0.0f;
	main_camera = new Camera(glm::vec3(1, 1, 1), glm::radians(70.0f));

	terrainShader = LoadShader("shaders/gourad.vert", "shaders/gourad.fraq");
	objectShader = LoadShader("shaders/gourad.vert", "shaders/gourad.fraq");
	if (terrainShader == nullptr || objectShader == nullptr)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		status = -1;
		return;
	}

	terrainTexture = LoadTexture("resources/img.png");
	objectTexture = LoadTexture("resources/img.png");
	if (terrainTexture == nullptr || objectTexture == nullptr)
	{
		std::cerr << "Failed to load texture" << std::endl;
		status = -1;
		return;
	}

	terrain = TerrainGenerator::CreateTerrain();
	terrainMesh = RenderMesh::RenderTerrainMesh(terrain);

	object = new PhysicalObject(glm::vec3(10, 5, 10), 2, 2, 2, 1);
	objectMesh = RenderMesh::RenderPhysicalObjectMesh(object);

	int n = 0;
	float** map = terrain->GetHeightMap(&n);
	PhysicsProcessing::InitPhysics(map, n, object);
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

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		HandleShaders();

		Window::SwapBuffers();
		Events::PollEvents();

		PhysicsProcessing::ComputePhysics(delta);
		delete objectMesh;
		objectMesh = RenderMesh::RenderPhysicalObjectMesh(object);

		//* FPS COUNTER
		if (++frames > 500)
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
		while (cps * clock() - currentTime < 1.0f / 140)
		{
			1;
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
	objectMesh->Draw(GL_TRIANGLES);
}

void GameScene::Final() 
{

}