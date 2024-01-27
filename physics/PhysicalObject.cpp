#include "PhysicalObject.h"

#include <iostream>

#include <glm/ext.hpp>

#include "Math.h"
#include "Collider.h"

PhysicalObject::PhysicalObject(glm::vec3 r, float a, float b, float c, float m) :
	M(m), r(r), v(0.0f), A(1.0f), L(0.0f), w(0.0f)
{
	M_1 = 1.0f / m;
	if (m == FLT_MAX) 
	{
		M_1 = 0.0f;
	}

	I_1 = m / 12.0f * glm::mat3(
		b * b + c * c, 0, 0,
		0, a * a + c * c, 0,
		0, 0, a * a + b * b);
	I_1 = glm::inverse(I_1);

	glm::vec3 x = glm::vec3(a / 2.0f, 0.0f, 0.0f);
	glm::vec3 y = glm::vec3(0.0f, b / 2.0f, 0.0f);
	glm::vec3 z = glm::vec3(0.0f, 0.0f, c / 2.0f);
	collider = new Collider(new glm::vec3[8]{
		glm::vec3(-x - y - z),
		glm::vec3(+x - y - z),
		glm::vec3(-x - y + z),
		glm::vec3(+x - y + z),
		glm::vec3(-x + y - z),
		glm::vec3(+x + y - z),
		glm::vec3(-x + y + z),
		glm::vec3(+x + y + z) }, 8);

	collider->RecalculatePoints(r, A);
}

PhysicalObject PhysicalObject::operator=(const PhysicalObject& object)
{
	PhysicalObject* obj = new PhysicalObject(object.r, 0, 0, 0, object.M);
	obj->I_1 = object.I_1;
	obj->collider = object.collider;
	return *(obj);
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

void PhysicalObject::CheckIfStatic()
{
	glm::mat3 I = A * I_1 * glm::transpose(A);
	glm::vec3 _w = I * L;

	if (0.5f * (M * glm::dot(v, v) + glm::dot(L, _w)) < 0.001f)
	{
		v = glm::vec3(0.0f);
		L = glm::vec3(0.0f);
	}
}

void PhysicalObject::Move(float dt)
{
	CheckIfStatic();

	r = r + v * dt;

	glm::mat3 I = A * I_1 * glm::transpose(A);
	glm::vec3 _w = I * L;
	w = glm::mat3(
		0.0f, _w.z, -_w.y,
		-_w.z, 0.0f, _w.x,
		_w.y, -_w.x, 0.0f);

	A = A + w * A * dt;
	A = Math::Reorthogonalize(A);

	float k = 0.00001f; //потеря энергии
	v -= k * v;
	L -= k * L;

	collider->RecalculatePoints(r, A);
}

void PhysicalObject::Move(glm::vec3 dir, float l)
{
	r += dir * l;

	collider->RecalculatePoints(r, A);
}
