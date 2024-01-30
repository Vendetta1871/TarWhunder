#ifndef EVENTS_H
#define EVENTS_H

typedef unsigned int uint;
/*===============================
Keyboard and mouse events handler
===============================*/
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
	static bool IsCursorLocked;
	static bool IsCursorStarted;

	static int Init();
	static void PollEvents();

	// returns if <keycode> key is currently pressed
	static bool IsKeyPressed(int keycode);
	// returns if <keycode> key was pressed in current frame
	static bool JustKeyPressed(int keycode);
	// returns if <button> button is currently clicked
	static bool IsMouseButtonClicked(int button);
	// returns if <button> button was clicked in current frame
	static bool JustMouseButtonClicked(int button);
	// toggle cursor visibility 
	static void ToggleCursor();
};

#endif
