#include "PhysicalObject.h"

#include "Math.h"

#include <glm/ext.hpp>

#include <iostream>

ICollidable::ICollidable(float a, float b, float c) : a(a), b(b), c(c)
{
}

PhysicalObject::PhysicalObject(glm::vec3 r, float a, float b, float c, float m) :
	ICollidable(a, b, c), M(m), r(r), v(0.0f), A(1.0f), L(0.0f), w(0.0f)
{
	I_1 = m / 12.0f * glm::mat3(
		b * b + c * c, 0, 0,
		0, a * a + c * c, 0,
		0, 0, a * a + b * b);
	I_1 = glm::inverse(I_1);

	RecalculateBounds();
}

void PhysicalObject::ApplyForce(glm::vec3 force, glm::vec3 point, float dt)
{
	v = v + force / M * dt;
	L = L + glm::cross(point - r, force) * dt;
}

void PhysicalObject::ApplyImpulse(glm::vec3 impulse, glm::vec3 point)
{
	v = v + impulse / M;
	L = L + glm::cross(point - r, impulse);
}

glm::mat3 Reorthogonalize(glm::mat3 m)
{
	glm::vec3 a = m[0];
	glm::vec3 b = m[1] - glm::dot(m[1], a) / glm::dot(a, a) * a;
	glm::vec3 c = m[2] - glm::dot(m[2], a) / glm::dot(a, a) * a - glm::dot(m[2], b) / glm::dot(b, b) * b;

	return glm::mat3(glm::normalize(a), glm::normalize(b), glm::normalize(c));
}

void PhysicalObject::Move(float dt)
{
	r = r + v * dt;

	glm::mat3 I = A * I_1 * glm::transpose(A);
	glm::vec3 _w = I * L;
	// ѕочему минус?? по формуле должен быть плюс
	w = -1.0f * glm::mat3(
		0.0f, -_w.z, _w.y,
		_w.z, 0.0f, -_w.x,
		-_w.y, _w.x, 0.0f);

	A = A + w * A * dt;
	A = Reorthogonalize(A);

	RecalculateBounds();
}

void PhysicalObject::RecalculateBounds()
{
	glm::vec3 oX = A * glm::vec3(a / 2, 0.0f, 0.0f);
	glm::vec3 oY = A * glm::vec3(0.0f, b / 2, 0.0f);
	glm::vec3 oZ = A * glm::vec3(0.0f, 0.0f, c / 2);

	Points[0] = r - oX - oY - oZ;
	Points[1] = r + oX - oY - oZ;
	Points[2] = r - oX - oY + oZ;
	Points[3] = r + oX - oY + oZ;
	Points[4] = r - oX + oY - oZ;
	Points[5] = r + oX + oY - oZ;
	Points[6] = r - oX + oY + oZ;
	Points[7] = r + oX + oY + oZ;

	BoundRect[0] = Points[0].x;
	BoundRect[1] = Points[0].z;
	BoundRect[2] = Points[0].x;
	BoundRect[3] = Points[0].z;
	for (int i = 1; i < 8; ++i)
	{
		if (Points[i].x < BoundRect[0])
		{
			BoundRect[0] = (int)glm::floor(Points[i].x);
		}
		if (Points[i].z < BoundRect[1])
		{
			BoundRect[1] = (int)glm::floor(Points[i].z);
		}
		if (Points[i].x > BoundRect[2])
		{
			BoundRect[2] = (int)glm::ceil(Points[i].x);
		}
		if (Points[i].z > BoundRect[3])
		{
			BoundRect[3] = (int)glm::ceil(Points[i].z);
		}
	}
}
