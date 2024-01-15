#include <iostream>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Events.h"
#include "Shader.h"
#include "Texture.h"

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

	while (!Window::GetShouldClose()) 
	{
		Events::PollEvents();
		if (Events::JustPressed(GLFW_KEY_ESCAPE))
		{
			Window::SetShouldClose(true);
		}

		glClear(GL_COLOR_BUFFER_BIT);

		shader->Use();
		texture->Bind();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		Window::SwapBuffers();
	}

	delete shader;
	delete texture;
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	Window::Terminate();
	return 0;
}
