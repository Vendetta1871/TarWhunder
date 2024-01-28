#include "Scene.h"
#include "GameScene.h"

#include <glm/glm.hpp>
#include <iostream>

#include <time.h>
#include <vector>
#include <list>

template <typename T>
class cont
{
public:
	unsigned int size = 1;
	unsigned int last_index = 0;
	T* array = new T[1];

	void push_back(T value)
	{
		if (last_index >= size)
		{
			T* buff = new T[size];
			memmove_s(buff, size * sizeof(T), array, size * sizeof(T));
			delete[] array;
			array = new T[size * 2];
			memmove_s(array, size * sizeof(T), buff, size * sizeof(T));
			delete[] buff;
			size *= 2;
		}
		array[last_index] = value;
		++last_index;
	}

	~cont()
	{
		delete[] array;
	}

	T& operator[](unsigned i) 
	{ 
		return T[i]; 
	}
};

int main() 
{
	/*
	int t = clock();
	t = clock();
	for (int j = 0; j < 10000; ++j)
	{
		cont<int> a;
		for (int i = 0; i < 1000; ++i)
		{
			a.push_back(i);
		}
	}
	std::cout << "array push_back: " << clock() - t << std::endl;

	t = clock();
	for (int j = 0; j < 10000; ++j)
	{
		std::vector<int> v;
		for (int i = 0; i < 1000; ++i)
		{
			v.push_back(1);
		}
	}
	std::cout << "vector push_back: " << clock() - t << std::endl;
	*/

	Scene* scene = new GameScene(1080, 720);
	if (scene->status)
	{
		return -1;
	}
	scene->Start();
	scene->Final();
	delete scene;

	return 0;
}
