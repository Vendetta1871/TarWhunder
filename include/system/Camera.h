#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
/*==========
Camera class
==========*/
class Camera
{
private:
	float fov;
	// recalculate local basis vectors in world coordinates
	void UpdateVectors();
public:
	glm::vec3 Up; // vector to the top from the direction of view
	glm::vec3 Front; // vector in the direction of view
	glm::vec3 Right; // vector to the right from the direction of view

	glm::vec3 Position;
	glm::mat4 Rotation;

	Camera(const glm::vec3& position, float fov);

	void Rotate(float x, float y, float z);

	glm::mat4 GetView();
	glm::mat4 GetProjection();
};

#endif
