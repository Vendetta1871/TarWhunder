#ifndef WINDOW_H
#define WINDOW_H

struct GLFWwindow;
/*===================
Window handling class
===================*/
class Window
{
public:
	static GLFWwindow* DefaultWindow; // openGL window object

	static int Width;
	static int Height;
	// instantiating of a window (returns 0 if successful)
	static int Init(int width, int height, const char* title);
	//TODO: add method to set up a window icon picture
	static void Terminate();
	// set cursor visibility
	static void SetCursorMode(int mode);
	// check if window closed
	static bool GetShouldClose();
	// set if window closed
	static void SetShouldClose(bool flag);
	// swap window buffers
	static void SwapBuffers();
};

#endif
