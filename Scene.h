#ifndef SCENE_H
#define SCENE_H

class Scene
{
protected:
	int width;
	int height;

public:
	int status;

	Scene(int width, int height);
	~Scene();

	virtual void Start() = 0;
	virtual void Final() = 0;
};

#endif
