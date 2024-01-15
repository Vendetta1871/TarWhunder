#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Window.h"
#include "Events.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Mesh.h"

int WIDTH = 1280;
int HEIGHT = 720;

float vertixes[] = {
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,

	0.5f,  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,  0.0f, -1.0f,
	-0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  0.0f, -1.0f,
	0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.0f,  0.0f,  1.0f,
	0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
	-0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,

	0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  0.0f,  1.0f,
	-0.5f, 0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
	//
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  -1.0f, 0.0f,
	0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f,
			   
	-0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  -1.0f, 0.0f,
	0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  -1.0f, 0.0f,
	0.5f,  -0.5f, 0.5f,  1.0f, 1.0f, 0.0f,  -1.0f, 0.0f,
			   
	-0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	-0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
			   
	-0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  -0.5f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	//
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	-0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
			    
	-0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, 0.5f,  0.0f, 1.0f, 1.0f,  0.0f,  0.0f,
	-0.5f, 0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  0.0f,  0.0f,
			    
	0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f,
	0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f,
	0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, -1.0f, 0.0f,  0.0f,
			    
	0.5f,  0.5f,  -0.5f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f, -1.0f, 0.0f,  0.0f,
	0.5f,  -0.5f, 0.5f,  0.0f, 1.0f, -1.0f, 0.0f,  0.0f,
};

int attrs[] = {
	3, 2, 3, 0
}; 

glm::vec3 lightPosition = glm::vec3(5.0f, 2.0f, 5.0f);

int main() 
{
	Window::Init(WIDTH, HEIGHT, "Window");
	Events::Init();

	Shader* shader = LoadShader("main.vert", "main.fraq");
	if (shader == nullptr)
	{
		std::cerr << "Failed to load shaders" << std::endl;
		Window::Terminate();
		return -1;
	}

	Texture* texture = LoadTexture("img.png");
	if (texture == nullptr)
	{
		std::cerr << "Failed to load texture" << std::endl;
		delete shader;
		Window::Terminate();
		return -1;
	}

	Mesh* mesh = new Mesh(vertixes, 36, attrs);

	glClearColor(117 / 255.0f, 187 / 255.0f, 253 / 255.0f, 1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(glm::vec3(0, 0, 1), glm::radians(70.0f));

	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.5f, 0, 0));

	float lastTime = glfwGetTime();
	float delta = 0.0f;

	float camX = 0.0f;
	float camY = 0.0f;

	float speed = 5;

	while (!Window::GetShouldClose()) 
	{
		float currentTime = glfwGetTime();
		delta = currentTime - lastTime;
		lastTime = currentTime;

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

		Window::SwapBuffers();
		Events::PollEvents();
	}

	delete shader;
	delete texture;
	delete mesh;

	Window::Terminate();
	return 0;
}
