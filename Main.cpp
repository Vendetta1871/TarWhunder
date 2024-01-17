#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "system/Window.h"
#include "system/Events.h"
#include "system/Camera.h"
#include "graphics/Shader.h"
#include "graphics/Texture.h"
#include "graphics/Mesh.h"
#include "graphics/RenderMesh.h"
#include "Terrain.h"

int WIDTH = 1080;
int HEIGHT = 720;

glm::vec3 lightPosition = glm::vec3(500.0f, 50.0f, 500.0f);

int main() 
{
	Window::Init(WIDTH, HEIGHT, "Window");
	Events::Init();

	Shader* shader = LoadShader(
		"shaders/gourad.vert", "shaders/gourad.fraq");
	Shader* shader1 = LoadShader(
		"shaders/gourad.vert", "shaders/gourad.fraq");
	if (shader == nullptr)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		Window::Terminate();
		return -1;
	}

	Texture* texture = LoadTexture("resources/img.png");
	if (texture == nullptr)
	{
		std::cerr << "Failed to load texture" << std::endl;
		delete shader;
		Window::Terminate();
		return -1;
	}

	//float t = glfwGetTime();
	Terrain* terrain = TerrainGenerator::CreateTerrain();

	Mesh* mesh = RenderMesh::RenderTerrainMesh(terrain);
	//Mesh* mesh = RenderMesh::RenderTerrain();
	//Mesh* mesh = RenderMesh::RenderTerrainBakedLight(lightPosition, 0.1f);
	//std::cout << glfwGetTime() - t << std::endl;

	glClearColor(117 / 255.0f, 187 / 255.0f, 253 / 255.0f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(glm::vec3(10, 10, 10), glm::radians(70.0f));

	glm::mat4 model(1.0f);

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	int frames = 0;
	float time = 0;

	while (!Window::GetShouldClose()) 
	{
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;
		/* FPS COUNTER
		if (++frames > 100) 
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
		//*
		float angle = glm::radians(delta * 100);
		glm::mat3x3 rot = glm::mat3x3(
			glm::cos(angle), 0, glm::sin(angle),
			0, 1, 0,
			-glm::sin(angle), 0, glm::cos(angle));
		lightPosition = lightPosition * rot;
		//*/

		if (Events::JustPressed(GLFW_KEY_ESCAPE))
		{
			Window::SetShouldClose(true);
		}
		if (Events::JustPressed(GLFW_KEY_TAB)) {
			Events::ToogleCursor();
		}

		if (Events::IsPressed(GLFW_KEY_W)) {
			camera->Position += camera->Front * delta * speed;
		}
		if (Events::IsPressed(GLFW_KEY_S)) {
			camera->Position -= camera->Front * delta * speed;
		}
		if (Events::IsPressed(GLFW_KEY_D)) {
			camera->Position += camera->Right * delta * speed;
		}
		if (Events::IsPressed(GLFW_KEY_A)) {
			camera->Position -= camera->Right * delta * speed;
		}
		if (Events::IsPressed(GLFW_KEY_SPACE)) {
			camera->Position += camera->Up * delta * speed;
		}
		if (Events::IsPressed(GLFW_KEY_LEFT_CONTROL)) {
			camera->Position -= camera->Up * delta * speed;
		}

		if (Events::IsCursorLocked) {
			camY += -Events::DeltaY / Window::Height * 2;
			camX += -Events::DeltaX / Window::Height * 2;

			if (camY < -glm::radians(89.0f)) {
				camY = -glm::radians(89.0f);
			}
			if (camY > glm::radians(89.0f)) {
				camY = glm::radians(89.0f);
			}

			camera->Rotation = glm::mat4(1.0f);
			camera->Rotate(camY, camX, 0);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->Use();		
		shader->UniformMatrix("model", model);
		shader->UniformMatrix("projview", 
			camera->GetProjection()*camera->GetView());
		shader->UniformVector("lightPos", lightPosition);
		texture->Bind();
		mesh->Draw(GL_TRIANGLES);

		shader1->Use();
		shader1->UniformMatrix("model", model);
		shader1->UniformMatrix("projview",
			camera->GetProjection() * camera->GetView());
		shader1->UniformVector("lightPos", lightPosition);

		Window::SwapBuffers();
		Events::PollEvents();
	}

	delete shader;
	delete texture;
	delete mesh;

	Window::Terminate();
	return 0;
}
