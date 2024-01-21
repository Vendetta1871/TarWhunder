#include "PhysicsProcessing.h"

#include "PhysicalObject.h"
#include "Math.h"

#include <iostream>

float** PhysicsProcessing::h_map;
int  PhysicsProcessing::h_map_size;
PhysicalObject* PhysicsProcessing::object;

void PhysicsProcessing::InitPhysics(float** height_map, int n, PhysicalObject* object)
{
	h_map = height_map;
	h_map_size = n;
	PhysicsProcessing::object = object;
}

int PhysicsProcessing::Collide(glm::vec3* point, glm::vec3* n, float* len)
{
	int count = 0;

	int x0 = glm::max(object->BoundRect[0], 0);
	int z0 = glm::max(object->BoundRect[1], 0);
	int x1 = glm::min(object->BoundRect[2], h_map_size);
	int z1 = glm::min(object->BoundRect[3], h_map_size);
	for (int x = x0; x < x1; ++x)
	{
		for (int z = z0; z < z1; ++z)
		{
			for (int i = 0; i < 8; ++i)
			{
				if (glm::round(object->Points[i].x) != x ||
					glm::round(object->Points[i].z) != z)
				{
					continue;
				}

				glm::vec3 normal = Math::GetPolygonNormal(h_map, h_map_size, x, z);


				float d = -(normal.x * x + normal.y * h_map[x][z] + normal.z * z);
				float y = - (normal.x * object->Points[i].x + 
					normal.z * object->Points[i].z + d) / normal.y;
				if (y <= object->Points[i].y)
				{
					continue;
				}

				point[count] = object->Points[i];

				normal = glm::normalize(normal);
				n[count] = normal;

				glm::vec3 line = glm::vec3(x, h_map[x][z], z) - object->Points[i];
				len[count] = glm::dot(line, normal);
				
				++count;
			}
		}
	}
	return count;
}

void PhysicsProcessing::ComputePhysics(float time)
{
	glm::vec3 gravity = object->M * glm::vec3(0, -0.098f, 0);
	object->ApplyForce(gravity, object->r, time);

	float len[8];
	glm::vec3 n[8];
	glm::vec3 point[8];
	int count = Collide(point, n, len);
	for (int i = 0; i < count; ++i)
	{
		glm::vec3 r = point[i] - object->r;
		glm::vec3 v = object->v + object->w * r;

		// проверка только дл€ столкновени€ с землей
		// дл€ других тел должна удовл. этому условию  ј∆ƒјя из скоростей
		if (glm::dot(v, n[i]) > 0) // если нет, то приложить импульс, если да - силу выталкивани€
		{
			// сила выталкивани€ из полигона
			float k = 0.3f;
			glm::vec3 react_force = k * len[i] * len[i] * object->M * n[i];
			object->ApplyForce(react_force, point[i], time);
			continue;
		}

		float a = glm::dot(n[i], n[i]) * (1.0f / object->M);
		glm::mat3 I = object->A * object->I_1 * glm::transpose(object->A);
		float b = glm::dot(glm::cross(I * glm::cross(r, n[i]), r), n[i]);

		float e = 0.55f; // коэф. упругого восстановалени€
		float j =  - (1.0f + e) * glm::dot(v, n[i]) / (a + b);

		glm::vec3 impulse = j * n[i];
		object->ApplyImpulse(impulse, point[i]);
	}

	// силы сопротивлени€

	object->Move(time);
}
