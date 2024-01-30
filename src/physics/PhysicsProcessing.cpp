#include "../../include/physics/PhysicsProcessing.h"
#include "../../include/physics/PhysicalObject.h"
#include "../../include/mathf/MathF.h"
#include "../../include/mathf/Collider.h"
#include "../../include/gameobject/Terrain.h"

#include <iostream>
#include <ctime>

std::vector<PhysicalObject*> PhysicsProcessing::objects;
const Terrain* PhysicsProcessing::terrain;

void PhysicsProcessing::InitPhysics(const Terrain* terrain, const std::vector<PhysicalObject*>& objects)
{
	PhysicsProcessing::terrain = terrain;
	PhysicsProcessing::objects = objects;
}
// spreading apart to prevent drowning bodies in each other
void PositionalCorrection(PhysicalObject* obj0, PhysicalObject* obj1, const glm::vec3& n, float depth)
{
	float percent = 0.2f;
	float slop = 0.05f; 
	float correction = std::max(depth - slop, 0.0f) / (obj0->InvMass + obj1->InvMass) * percent;
	obj0->Move(n, obj0->InvMass * correction);
	obj1->Move(-n, obj1->InvMass * correction);
}
// apply collision impulses
void ResolveCollision(PhysicalObject* obj0, PhysicalObject* obj1, const glm::vec3& n, const glm::vec3& p)
{
	glm::mat3 I0 = obj0->Rotation * obj0->InvInertia * glm::transpose(obj0->Rotation); // inertia tensor in world coordinates
	glm::mat3 I1 = obj1->Rotation * obj1->InvInertia * glm::transpose(obj1->Rotation); // inertia tensor in world coordinates
	glm::vec3 r0 = p - obj0->Position; // vector from the center of body to collision point 
	glm::vec3 r1 = p - obj1->Position; // vector from the center of body to collision point 
	
	glm::vec3 v = obj0->GetPointVelocity(r0) - obj1->GetPointVelocity(r1); // relative velocity

	float v_n = glm::dot(v, n);
	if (v_n > 0.0f) // check if bodies are moving away from each other
	{
		return;
	}
	// the denominator of the fraction in impulse value
	float a = glm::dot(I0 * glm::cross(glm::cross(r0, n), r0) + I1 * glm::cross(glm::cross(r1, n), r1), n) + obj0->InvMass + obj1->InvMass;

	glm::vec3 t = MathF::Normalize(v - v_n * n);
	t = (t.x == t.x) ? t : glm::vec3(0.0f); // check if it velocity perpendicular collision normal

	float e = 0.35f; // elasticity coefficient
	float mu = 0.25f; // friction coefficient
	float j_r = -(1.0f + e) * v_n / a; // reaction force impulse
	float j_f = -1.0f * mu * j_r; // friction impulse
	obj0->ApplyImpulse(j_r * n + j_f * t, p);
	obj1->ApplyImpulse(-(j_r * n + j_f * t), p);
}

void PhysicsProcessing::SimulatePhysics(float time)
{
	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		//TODO: friction forces
		glm::vec3 forces = glm::vec3(0.0f); 
		forces += 1.0f / objects[i]->InvMass * glm::vec3(0, -0.2f, 0); // gravity
		objects[i]->ApplyForce(forces, objects[i]->Position, time);
		objects[i]->Move(time);
	}
	// search and resolve all collisions
	for (unsigned int i = 0; i < objects.size(); ++i)
	{
		glm::vec3 normal;
		glm::vec3 point;
		float depth;
		if (objects[i]->BoxCollider->IsTerrainCollision(terrain, normal, point, depth))
		{
			ResolveCollision(objects[i], terrain->Object, normal, point);
			PositionalCorrection(objects[i], terrain->Object, normal, depth);
		}

		for (unsigned int j = i + 1; j < objects.size(); ++j)
		{
			if (objects[i]->BoxCollider->SAT(objects[j]->BoxCollider, normal, point, depth))
			{
				ResolveCollision(objects[j], objects[i], normal, point);
				PositionalCorrection(objects[j], objects[i], normal, depth);
			}
		}
	}
}
