#include "../../include/system/Window.h"

#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* Window::DefaultWindow;
int Window::Width;
int Window::Height;

int Window::Init(int width, int height, const char* title) 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	DefaultWindow = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (DefaultWindow == nullptr) 
	{
		std::cerr << "Failed to create GLFW Window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(DefaultWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) 
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, width, height);

	Window::Width = width;
	Window::Height = height;
	return 0;
}

void Window::Terminate() 
{
	glfwTerminate();
}

void Window::SetCursorMode(int mode) {
	glfwSetInputMode(DefaultWindow, GLFW_CURSOR, mode);
}

bool Window::GetShouldClose() 
{
	return glfwWindowShouldClose(DefaultWindow);
}

void Window::SetShouldClose(bool flag) {
	glfwSetWindowShouldClose(DefaultWindow, flag);
}

void Window::SwapBuffers() 
{
	glfwSwapBuffers(DefaultWindow);
}
