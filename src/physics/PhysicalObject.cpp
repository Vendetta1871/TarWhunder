#include "../../include/physics/PhysicalObject.h"
#include "../../include/mathf/MathF.h"
#include "../../include/mathf/Collider.h"

#include <iostream>

#include <glm/ext.hpp>

PhysicalObject::PhysicalObject(const glm::vec3& position, float a, float b, float c, float mass) : 
	Position(position), Velocity(0.0f), Rotation(1.0f), AngularMomentum(0.0f), angularVelocity(0.0f)
{
	// check if infinity mass object
	InvMass = std::max(1.0f / mass, 0.0f); 

	glm::mat3 inertia = glm::mat3(b * b + c * c, 0, 0, 0, a * a + c * c, 0, 0, 0, a * a + b * b);
	InvInertia = (mass > 0.0f) ? glm::inverse(mass / 12.0f * inertia) : glm::mat3(0.0f);

	BoxCollider = new Collider(a, b, c);
	BoxCollider->UpdateOBB(Position, Rotation);
}

PhysicalObject::~PhysicalObject()
{
	delete BoxCollider;
}

void PhysicalObject::SleepIfStatic()
{
	glm::mat3 I = Rotation * InvInertia * glm::transpose(Rotation); // inertia tensor in world coordinates
	glm::vec3 w = I * AngularMomentum; // angular speed in world coordinates
	// kinetic energy of body
	float e = 0.5f * (1.0f / InvMass * glm::dot(Velocity, Velocity) + glm::dot(AngularMomentum, w)); 
	if (e < 0.001f) // check if energy less than threshold
	{
		Velocity = glm::vec3(0.0f);
		AngularMomentum = glm::vec3(0.0f);
	}
}

void PhysicalObject::ApplyForce(const glm::vec3& force, const glm::vec3& point, float dt)
{
	Velocity += force * InvMass * dt;
	AngularMomentum += glm::cross(point - Position, force) * dt;
}

void PhysicalObject::ApplyImpulse(const glm::vec3& impulse, const glm::vec3& point)
{
	Velocity += impulse * InvMass;
	AngularMomentum += glm::cross(point - Position, impulse);
}

void PhysicalObject::Move(float dt)
{
	SleepIfStatic();

	Position += Velocity * dt;

	glm::mat3 I = Rotation * InvInertia * glm::transpose(Rotation); // inertia tensor in world coordinates
	glm::vec3 w = I * AngularMomentum; // angular speed in world coordinates
	angularVelocity = glm::mat3(
		0.0f, w.z, -w.y,
		-w.z, 0.0f, w.x,
		w.y, -w.x, 0.0f);

	Rotation += angularVelocity * Rotation * dt;
	Rotation = MathF::Orthonormalize(Rotation);

	//TODO: replace energy losses with friction forces
	float k = 0.00001f;
	Velocity -= k * Velocity;
	AngularMomentum -= k * AngularMomentum;

	BoxCollider->UpdateOBB(Position, Rotation);
}

void PhysicalObject::Move(const glm::vec3& direction, float range)
{
	Position += direction * range;

	BoxCollider->UpdateOBB(Position, Rotation);
}

glm::vec3 PhysicalObject::GetPointVelocity(const glm::vec3& point)
{
	return Velocity + angularVelocity * point;
}
