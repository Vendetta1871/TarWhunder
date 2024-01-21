#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include <glm/glm.hpp>

class ICollidable
{
protected:
	float a;
	float b;
	float c;

	virtual void RecalculateBounds() = 0;
public:
	ICollidable(float a, float b, float c);

	glm::vec3 Points[8];
	int BoundRect[4];
};

class PhysicalObject : public ICollidable
{
private:
	void RecalculateBounds();

public:
	const int type = 0;

	float M;
	glm::mat3 I_1;

	glm::vec3 r;
	glm::vec3 v;
	glm::mat3 A;
	glm::vec3 L;

	glm::mat3 w;

	PhysicalObject(glm::vec3 r, float a, float b, float c, float m);

	void ApplyForce(glm::vec3 force, glm::vec3 point, float dt);
	void ApplyImpulse(glm::vec3 impulse, glm::vec3 point);

	void Move(float dt);
};

#endif
