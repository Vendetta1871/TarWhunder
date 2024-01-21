#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;

class Window
{
public:
	static GLFWwindow* window;

	static int Width;
	static int Height;

	static int Init(int width, int height, const char* title);
	static void Terminate();

	static void SetCursorMode(int mode);
	static bool GetShouldClose();
	static void SetShouldClose(bool flag);

	static void SwapBuffers();
};

#endif
