#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera
{
private:
	float fov;
	
	void UpdateVectors();
public:
	glm::vec3 Position;
	glm::mat4 Rotation;

	glm::vec3 Front;
	glm::vec3 Right;
	glm::vec3 Up;

	Camera(glm::vec3 position, float fov);

	void Rotate(float x, float y, float z);

	glm::mat4 GetProjection();
	glm::mat4 GetView();
};

#endif
