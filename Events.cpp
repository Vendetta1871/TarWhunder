#include <GLFW/glfw3.h>
#include <string.h>

#include "Events.h"
#include "Window.h"

bool* Events::Keys;
uint* Events::Frames;
uint Events::Current = 0;
float Events::DeltaX = 0.0f;
float Events::DeltaY = 0.0f;
float Events::X = 0.0f;
float Events::Y = 0.0f;
bool Events::CursorLocked = false;
bool Events::CursorStarted = false;

#define _MOUSE_BUTTONS 1024

void CursorPositionCallback(GLFWwindow* window, double xpos, double ypos) 
{
	if (Events::CursorStarted) 
	{
		Events::DeltaX += xpos - Events::X;
		Events::DeltaY += ypos - Events::Y;
	}
	else 
	{
		Events::CursorStarted = true;
	}
	Events::X = xpos;
	Events::Y = ypos;
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mode) 
{
	if (action == GLFW_PRESS) 
	{
		Events::Keys[_MOUSE_BUTTONS + button] = true;
		Events::Frames[_MOUSE_BUTTONS + button] = Events::Current;
	}
	else if (action == GLFW_RELEASE) 
	{
		Events::Keys[_MOUSE_BUTTONS + button] = false;
		Events::Frames[_MOUSE_BUTTONS + button] = Events::Current;
	}
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) 
{
	if (action == GLFW_PRESS) 
	{
		Events::Keys[key] = true;
		Events::Frames[key] = Events::Current;
	}
	else if (action == GLFW_RELEASE) 
	{
		Events::Keys[key] = false;
		Events::Frames[key] = Events::Current;
	}
}

int Events::Init() 
{
	GLFWwindow* window = Window::window;
	Keys = new bool[1032];
	Frames = new uint[1032];

	memset(Keys, false, 1032 * sizeof(bool));
	memset(Frames, 0, 1032 * sizeof(uint));

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	return 0;
}

bool Events::IsPressed(int keycode) 
{
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
	{
		return false;
	}
	return Keys[keycode];
}

bool Events::JustPressed(int keycode) {
	if (keycode < 0 || keycode >= _MOUSE_BUTTONS)
	{
		return false;
	}
	return Keys[keycode] && Frames[keycode] == Current;
}

bool Events::IsClicked(int button) 
{
	int index = _MOUSE_BUTTONS + button;
	return Keys[index];
}

bool Events::JustClicked(int button) 
{
	int index = _MOUSE_BUTTONS + button;
	return Keys[index] && Frames[index] == Current;
}

void Events::PollEvents() 
{
	Current++;
	DeltaX = 0.0f;
	DeltaY = 0.0f;
	glfwPollEvents();
}