#include "../../include/system/Camera.h"
#include "../../include/system/Window.h"

#include <glm/ext.hpp>

Camera::Camera(const glm::vec3& position, float fov) : Position(position), fov(fov), Rotation(1.0f) 
{
	UpdateVectors();
}

void Camera::UpdateVectors()
{
	Up = glm::vec3(Rotation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	Front = glm::vec3(Rotation * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
	Right = glm::vec3(Rotation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
}

void Camera::Rotate(float x, float y, float z) 
{
	Rotation = glm::rotate(Rotation, z, glm::vec3(0.0f, 0.0f, 1.0f));
	Rotation = glm::rotate(Rotation, y, glm::vec3(0.0f, 1.0f, 0.0f));
	Rotation = glm::rotate(Rotation, x, glm::vec3(1.0f, 0.0f, 0.0f));

	UpdateVectors();
}

glm::mat4 Camera::GetView()
{
	return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetProjection() 
{
	float aspect = (float)Window::Width / (float)Window::Height;
	return glm::perspective(fov, aspect, 0.1f, 100.0f);
}
