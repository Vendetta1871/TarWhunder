#include "Camera.h"
#include "Window.h"

#include <glm/ext.hpp>

Camera::Camera(glm::vec3 position, float fov) : Position(position), fov(fov), Rotation(1.0f) {
	UpdateVectors();
}

void Camera::UpdateVectors() {
	Front = glm::vec3(Rotation * glm::vec4(0, 0, -1, 1));
	Right = glm::vec3(Rotation * glm::vec4(1, 0, 0, 1));
	Up = glm::vec3(Rotation * glm::vec4(0, 1, 0, 1));
}

void Camera::Rotate(float x, float y, float z) {
	Rotation = glm::rotate(Rotation, z, glm::vec3(0, 0, 1));
	Rotation = glm::rotate(Rotation, y, glm::vec3(0, 1, 0));
	Rotation = glm::rotate(Rotation, x, glm::vec3(1, 0, 0));

	UpdateVectors();
}

glm::mat4 Camera::GetProjection() {
	float aspect = (float)Window::Width / (float)Window::Height;
	return glm::perspective(fov, aspect, 0.1f, 100.0f);
}

glm::mat4 Camera::GetView() {
	return glm::lookAt(Position, Position + Front, Up);
}

