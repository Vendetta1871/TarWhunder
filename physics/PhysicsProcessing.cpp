#include "PhysicsProcessing.h"

#include "PhysicalObject.h"
#include "Math.h"
#include "Collider.h"

#include <iostream>

CollisionInfo::CollisionInfo(glm::vec3 p, glm::vec3 n, PhysicalObject* obj0, PhysicalObject* obj1) : 
	p(p), n(n)
{
	this->obj0 = obj0;
	this->obj1 = obj1;
}


CollisionInfo::~CollisionInfo()
{
	//delete[] obj;
}

float** PhysicsProcessing::h_map;
int  PhysicsProcessing::h_map_size;
std::vector<PhysicalObject*> PhysicsProcessing::object;
PhysicalObject* PhysicsProcessing::terrain;

void PhysicsProcessing::InitPhysics(float** height_map, int n, std::vector<PhysicalObject*>* object)
{
	h_map = height_map;
	h_map_size = n;
	PhysicsProcessing::object = *object;
	terrain = new PhysicalObject(glm::vec3(0.0f), FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);
	terrain->I_1 = glm::mat3(0.0f);
}

void PositionalCorrection(PhysicalObject* A, PhysicalObject* B, glm::vec3 n, float depth)
{
	float percent = 0.2; // обычно от 20% до 80%
	float slop = 0.01; // обычно от 0.01 до 0.1
	float correction = std::max(depth - slop, 0.0f) / (A->M_1 + B->M_1) * percent;
	A->r -= A->M_1 * correction * n;
	B->r += B->M_1 * correction * n;
}

void ResolveCollision(PhysicalObject* obj0, PhysicalObject* obj1, glm::vec3 n, glm::vec3 p)
{
	glm::mat3 I0 = obj0->A * obj0->I_1 * glm::transpose(obj0->A);
	glm::mat3 I1 = obj1->A * obj1->I_1 * glm::transpose(obj1->A);
	glm::vec3 r0 = p - obj0->r;
	glm::vec3 r1 = p - obj1->r;
	float a = glm::dot(I0 * glm::cross(glm::cross(r0, n), r0) +
		I1 * glm::cross(glm::cross(r1, n), r1), n) + obj0->M_1 + obj1->M_1;

	glm::vec3 v1 = obj0->v + obj0->w * r0;
	glm::vec3 v2 = obj1->v + obj1->w * r1;
	glm::vec3 v_r = v1 - v2;

	float v_n = glm::dot(v_r, n);
	if (v_n > 0.0f)
	{
		return;
	}

	float e = 0.55f; // коэф. упругого восстановаления
	float j_r = -(1.0f + e) * v_n / a;
	obj0->ApplyImpulse(j_r * n, p);
	obj1->ApplyImpulse(-j_r * n, p);

	// трение при ударе
	//float mu_s = 0.2f;
	float mu_d = 0.15f;
	float j_f = -1.0f * mu_d * j_r;
	glm::vec3 t = Math::Normalize(v_r - v_n * n);
	if (t.x != t.x || t.y != t.y || t.z != t.z)
	{
		t = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	obj0->ApplyImpulse(j_f * t, p);
	obj1->ApplyImpulse(-j_f * t, p);
}

bool PhysicsProcessing::Collide(float dt)
{
	std::vector<CollisionInfo> collisions;

	for (int i = 0; i < object.size(); ++i)
	{
		int x0 = glm::max(object[i]->collider->BoundRect[0], 0);
		int y0 = object[i]->collider->BoundRect[1];
		int z0 = glm::max(object[i]->collider->BoundRect[2], 0);
		int x1 = glm::min(object[i]->collider->BoundRect[3], h_map_size - 1);
		int y1 = object[i]->collider->BoundRect[4];
		int z1 = glm::min(object[i]->collider->BoundRect[5], h_map_size - 1);

		bool flag = false;
		for (int x = x0; x < x1; ++x)
		{
			for (int z = z0; z < z1; ++z)
			{
				if (h_map[x][z] < y0 && h_map[x + 1][z] < y0 &&
					h_map[x][z + 1] < y0 && h_map[x + 1][z + 1] < y0)
				{
					continue;
				}

				float depth;
				auto info = Math::GetCollisionPoints(h_map, h_map_size, object[i]->collider->Points, x0, x1, z0, z1, depth);
				if (depth > 0.0f)
				{
					ResolveCollision(terrain, object[i], -info.second, info.first);
					PositionalCorrection(terrain, object[i], info.second, depth);
					flag = true;
					break;
				}
			}
			if (flag)
			{
				break;
			}
		}

		for (int j = i + 1; j < object.size(); ++j)
		{
			if ((object[i]->collider->BoundRect[0] > object[j]->collider->BoundRect[3]) ||
				(object[i]->collider->BoundRect[1] > object[j]->collider->BoundRect[4]) ||
				(object[i]->collider->BoundRect[2] > object[j]->collider->BoundRect[5]) ||
				(object[i]->collider->BoundRect[3] < object[j]->collider->BoundRect[0]) ||
				(object[i]->collider->BoundRect[4] < object[j]->collider->BoundRect[1]) ||
				(object[i]->collider->BoundRect[5] < object[j]->collider->BoundRect[2]))
			{
				continue;
			}

			float depth;
			glm::vec3 normal;
			if (!Math::SAT_OBBOBB(object[i]->collider->Points, object[j]->collider->Points, normal, depth))
			{
				continue;
			}

			glm::vec3 point = Math::GetCollisionPoints_OBBOBB(object[i]->collider->Points, object[j]->collider->Points, normal);
			ResolveCollision(object[i], object[j], -normal, point);
			PositionalCorrection(object[i], object[j], normal, depth);
		}
	}

	return true;
}

void PhysicsProcessing::ComputePhysics(float time)
{
	for (int i = 0; i < object.size(); ++i)
	{
		glm::vec3 forces = glm::vec3(0.0f);
		forces += object[i]->M * glm::vec3(0, -0.2f, 0); // gravity
		object[i]->ApplyForce(forces, object[i]->r, time);
		object[i]->Move(time);
	}
	
	//Link:
	//auto collisions = 
	Collide(time);
	//if (collisions.size() > 0) goto Link;

	// силы сопротивления
	for (int i = 0; i < object.size(); ++i)
	{
		
	}
}
