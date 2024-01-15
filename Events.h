#ifndef EVENTS_H
#define EVENTS_H

#include "Window.h"

typedef unsigned int uint;

class Events
{
public:
	static bool* Keys;
	static uint* Frames;
	static uint Current;
	static float DeltaX;
	static float DeltaY;
	static float X;
	static float Y;
	static bool CursorLocked;
	static bool CursorStarted;

	static int Init();
	static void PollEvents();

	static bool IsPressed(int keycode);
	static bool JustPressed(int keycode);

	static bool IsClicked(int button);
	static bool JustClicked(int button);
};

#endif