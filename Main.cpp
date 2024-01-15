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

int WIDTH = 1280;
int HEIGHT = 720;

float vertixes[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};

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

	GLuint VAO, VBO; 
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertixes), vertixes, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Camera* camera = new Camera(glm::vec3(0, 0, 1), glm::radians(70.0f));

	glm::mat4 model(1.0f);
	//model = glm::scale(model, glm::vec3(2, 2, 2));
	//model = glm::rotate(model, 0.5f, glm::vec3(0, 0, 1));
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

		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();
		shader->UniformMatrix("model", model);
		shader->UniformMatrix("projview", 
			camera->GetProjection()*camera->GetView());
		texture->Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		Window::SwapBuffers();
		Events::PollEvents();
	}

	delete shader;
	delete texture;
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	Window::Terminate();
	return 0;
}
