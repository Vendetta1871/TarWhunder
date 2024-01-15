#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Events.h"

int WIDTH = 1280;
int HEIGHT = 720;

int main() 
{
	Window::Init(WIDTH, HEIGHT, "Window");
	Events::Init();

	while (!Window::GetShouldClose()) 
	{
		Events::PollEvents();
		if (Events::JustPressed(GLFW_KEY_ESCAPE))
		{
			Window::SetShouldClose(true);
		}

		Window::SwapBuffers();
	}
	Window::Terminate();
	return 0;
}
