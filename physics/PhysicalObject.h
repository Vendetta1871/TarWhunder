#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include <glm/glm.hpp>

class Collider;

class PhysicalObject
{
private:
	void CheckIfStatic();

public:
	const int type = 0;

	float M;
	float M_1;
	glm::mat3 I_1;

	glm::vec3 r;
	glm::vec3 v;
	glm::mat3 A;
	glm::vec3 L;

	glm::mat3 w;

	Collider* collider;

	PhysicalObject operator=(const PhysicalObject& object);
	PhysicalObject(glm::vec3 r, float a, float b, float c, float m);

	void ApplyForce(glm::vec3 force, glm::vec3 point, float dt);
	void ApplyImpulse(glm::vec3 impulse, glm::vec3 point);

	void Move(float dt);
	void Move(glm::vec3 dir, float l);
};

#endif
