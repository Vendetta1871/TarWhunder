#ifndef SCENE_H
#define SCENE_H
/*=========================
Base scene class (abstract)
=========================*/
class Scene
{
protected:
	int width; // width of window
	int height; // height of window

public:
	int status; // status of instantiating of scene (0 if success)

	Scene(int width, int height);
	~Scene();
	// main loop of scene
	virtual void Start() = 0;
};

#endif
