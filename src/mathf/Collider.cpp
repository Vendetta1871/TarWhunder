#include "../../include/mathf/Collider.h"
#include "../../include/mathf/MathF.h"
#include "../../include/gameobject/Terrain.h"

#include <math.h>
#include <iostream>

#define M_PI 3.14159265358979323846f

std::vector<glm::vec3> Collider::Collisions;

Collider::Collider(float a, float b, float c)
{
	glm::vec3 x = glm::vec3(a / 2.0f, 0.0f, 0.0f);
	glm::vec3 y = glm::vec3(0.0f, b / 2.0f, 0.0f);
	glm::vec3 z = glm::vec3(0.0f, 0.0f, c / 2.0f);
	                              /************************************/
	vertexes = new glm::vec3[8]{  /*                 4_____________5  */
		glm::vec3(-x - y - z),    /*    y            /|           /|  */
		glm::vec3(+x - y - z),    /*    ^           / |          / |  */
		glm::vec3(-x - y + z),    /*    |         6/___________7/  |  */
		glm::vec3(+x - y + z),    /*    |         |   |        |   |  */
		glm::vec3(-x + y - z),    /*    |----> x  |   |        |   |  */
		glm::vec3(+x + y - z),    /*   /          |   |________|___|  */
		glm::vec3(-x + y + z),    /*  /           |  /0        | 1/   */
		glm::vec3(+x + y + z) };  /* z            | /          | /	  */
	                              /*              |/___________|/	  */
	OBB = new glm::vec3[8];       /*              2            3      */
	AABB = new glm::vec3[2];      /************************************/
}

Collider::~Collider()
{
	delete[] vertexes;
	delete[] OBB;
	delete[] AABB;
}

void Collider::UpdateAABB()
{
	AABB[0] = OBB[0];
	AABB[1] = OBB[0];

	for (int i = 1; i < 8; ++i)
	{
		AABB[0].x = std::min(OBB[i].x, AABB[0].x);
		AABB[0].y = std::min(OBB[i].y, AABB[0].y);
		AABB[0].z = std::min(OBB[i].z, AABB[0].z);
		AABB[1].x = std::max(OBB[i].x, AABB[1].x);
		AABB[1].y = std::max(OBB[i].y, AABB[1].y);
		AABB[1].z = std::max(OBB[i].z, AABB[1].z);
	}
}

int Collider::GetFurthestPointIndex(const glm::vec3& direction) const
{
	int ind_max = 0;
	float max = FLT_MIN;
	for (int i = 0; i < 8; ++i)
	{
		float p = glm::dot(OBB[i], direction);
		if (p >= max)
		{
			max = p;
			ind_max = i;
		}
	}
	return ind_max;
}

// indexes of OBB points of one edge 
const int faces[][4] = {
	{0, 1, 3, 2},
	{0, 4, 5, 1},
	{2, 6, 4, 0},
	{2, 3, 7, 6},
	{3, 1, 5, 7},
	{4, 6, 7, 5} };

bool Collider::IntersectLineOBB(const glm::vec3& o, const glm::vec3& m, glm::vec3& point) const
{
	for (auto& i : faces)
	{
		glm::vec3 a = OBB[i[0]];
		glm::vec3 b = OBB[i[1]];
		glm::vec3 c = OBB[i[2]];
		glm::vec3 d = OBB[i[3]];
		// normal of the plane of the face 
		glm::vec3 n = MathF::Normalize(glm::cross(c - a, b - a));
		// projections of <o> and <m> points on the plane of the face 
		glm::vec3 o1 = o - glm::dot(o - a, n) * n;
		glm::vec3 m1 = m - glm::dot(m - a, n) * n;                  /*********************************************/
		                                                            /*    o 									 */
		float y0 = glm::dot(o - o1, n);                             /*    |\ 									 */
		float y1 = glm::dot(m - m1, n);                             /*    | \  									 */
		// check if point are in different half-spaces from plane   /* y0 |  \ 									 */
		if (y0 * y1 > 0.0f)                                         /*    |   \ 						    	 */
		{                                                           /*    |    \ 								 */
			continue;                                               /* ===o1===point===m1=== (plane of the face) */
		}                                                           /*            \    |                         */
		y0 = std::abs(y0);                                          /*             \   |					     */
		y1 = std::abs(y1);                                          /*              \  | y1						 */
		// find an intersection point using similarity of triangles /*               \ |						 */
		point = o1 + y0 / (y0 + y1) * (m1 - o1);                    /*                \|						 */
		// sum of angles between vertexes and point                 /*                 m						 */
		float sum_angle = MathF::GetAngle(a, b, point);             /*********************************************/
		sum_angle += MathF::GetAngle(b, c, point);
		sum_angle += MathF::GetAngle(c, d, point);
		sum_angle += MathF::GetAngle(d, a, point);
		// if this angle are less than 2 * pi than point is outside square and opposite
		if (std::abs(2.0f * M_PI - sum_angle) < 0.01f)
		{
			return true;
		}
	}

	return false;
}
 
// each int[3] array contain numbers of points which in the same edge as first index
// ex. GetCubeNormal[0] = {1, 2, 4}; => cube edges is OBB[1]-OBB[0], OBB[2]-OBB[0] and OBB[4]-OBB[0]
const int edges[][3] = {
	{1, 2, 4}, 
	{0, 3, 5}, 
	{0, 3, 6}, 
	{1, 2, 7}, 
	{0, 5, 6}, 
	{1, 4, 7}, 
	{2, 4, 7}, 
	{3, 5, 6}};

glm::vec3 Collider::GetContactPoint(const Collider* obj, const glm::vec3& n) const
{
	// check for a vertex-face intersection
	glm::mat3 A = glm::inverse(glm::mat3(                 /*************************************************************/  
		OBB[1] - OBB[0],                                  /* Translating one OBB into other's local coordinates and    */
		OBB[2] - OBB[0],                                  /*	check if it's vertexes coordinates inside 1-sized cube */
		OBB[4] - OBB[0]));                                /*                                                           */
	                                                      /*                  (0, 1, 0)                                */
	for (int i = 0; i < 8; ++i)                           /*                      _____________                        */
	{                                                     /*                     /|           /|                       */
		glm::vec3 p = A * (obj->OBB[i] - OBB[0]);         /*                    / |          / |                       */
		if (p.x <= 1.0f && p.y <= 1.0f && p.z <= 1.0f &&  /*                   /____________/  |					   */
			p.x >= 0.0f && p.y >= 0.0f && p.z >= 0.0f)    /*                  |   |        |   | 					   */   
		{                                                 /*                  |   |        |   |				  	   */   
			return obj->OBB[i];                           /*                  |   |________|___|  (1, 0, 0)		       */   
		}                                                 /*                  |  /         |  /		     			   */   
	}                                                     /*                  | /          | /						   */ 
	// if there no vertexes of cube A inside cube B,      /*                  |/___________|/						   */
	// search if vertexes of B inside A                   /*              (0, 0, 1)                                    */
	A = glm::inverse(glm::mat3(                           /*                                                           */
		obj->OBB[1] - obj->OBB[0],                        /*************************************************************/
		obj->OBB[2] - obj->OBB[0],                        
		obj->OBB[4] - obj->OBB[0]));

	for (int i = 0; i < 8; ++i)
	{
		glm::vec3 p = A * (OBB[i] - obj->OBB[0]);
		if (p.x <= 1.0f && p.y <= 1.0f && p.z <= 1.0f &&
			p.x >= 0.0f && p.y >= 0.0f && p.z >= 0.0f)
		{
			return OBB[i];
		}
	}
	// check if its an edge-edge intersection
	glm::vec3 p = glm::vec3(0.0f);
	int index = GetFurthestPointIndex(n);
	for (auto& i : edges[index])
	{
		if (obj->IntersectLineOBB(OBB[index], OBB[i], p))
		{
			return p;
		}
	}

	index = obj->GetFurthestPointIndex(-n);
	for (auto& i : edges[index])
	{
		if (IntersectLineOBB(obj->OBB[index], obj->OBB[i], p))
		{
			return p;
		}
	}
	// return (nan, nan, nan) vector if it isn't intersection
	return MathF::Normalize(glm::vec3(0.0f));
}

void Collider::UpdateOBB(const glm::vec3& position, const glm::mat3& rotation)
{
	for (int i = 0; i < 8; ++i)
	{
		OBB[i] = position + rotation * vertexes[i];
	}

	UpdateAABB();
}

bool Collider::SAT(const Collider* obj, glm::vec3& normal, glm::vec3& point, float& depth) const
{
	normal = glm::vec3(0.0f);
	point = glm::vec3(0.0f);
	depth = FLT_MAX;

	if ((AABB[0].x > obj->AABB[1].x) || (AABB[1].x < obj->AABB[0].x) ||
		(AABB[0].y > obj->AABB[1].y) || (AABB[1].y < obj->AABB[0].y) ||
		(AABB[0].z > obj->AABB[1].z) || (AABB[1].z < obj->AABB[0].z))
	{
		return false; // if bounding AABB don't intersects, OBB also can't
	}

	int count = 6;
	// write faces normal to separating axis buffer 
	glm::vec3 axis[15] = { 
		MathF::Normalize(OBB[1] - OBB[0]),
		MathF::Normalize(OBB[2] - OBB[0]),
		MathF::Normalize(OBB[4] - OBB[0]),
		MathF::Normalize(obj->OBB[1] - obj->OBB[0]),
		MathF::Normalize(obj->OBB[2] - obj->OBB[0]),
		MathF::Normalize(obj->OBB[4] - obj->OBB[0]) };
	// add cross products if it isn't zero vector
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 3; j < 6; ++j)
		{
			glm::vec3 vec = glm::cross(axis[i], axis[j]);
			axis[count] = MathF::Normalize(vec);
			count += (int)(axis[count].x == axis[count].x); // check for zero vector
		}
	}
	// search for separating axis
	for (int i = 0; i < count; ++i)
	{
		// projecting vertexes into axis
		float max_a = FLT_MIN, max_b = FLT_MIN;
		float min_a = FLT_MAX, min_b = FLT_MAX;
		for (int j = 0; j < 8; ++j)
		{
			float p = glm::dot(OBB[j], axis[i]);
			max_a = std::max(max_a, p);
			min_a = std::min(min_a, p);
			p = glm::dot(obj->OBB[j], axis[i]);
			max_b = std::max(max_b, p);
			min_b = std::min(min_b, p);
		}
		// check if projections overlap
		float l = std::max(max_a, max_b) - std::min(min_a, min_b);
		float sum = (max_a - min_a) + (max_b - min_b);
		float d = sum - l;
		if (d < 0.0f)
		{
			return false;
		}
		// minimal overlap axis is an collision normal
		if (d <= depth)
		{
			depth = d;
			normal = axis[i];
			// making normal directed into the second body 
			if (max_a > max_b)
			{
				normal *= -1.0f;
			}
		}
	}

	normal = MathF::Normalize(normal);
	point = GetContactPoint(obj, normal);
	//TODO: move to debug class
	Collisions.push_back(point);
	// check if this->GetContactPoint() found a point
	return point.x == point.x;
}

bool Collider::IsTerrainCollision(const Terrain* terrain, glm::vec3& normal, glm::vec3& point, float& depth) const
{
	int count = 0; // find all collision points, normal and depths and calculate one resulting
	glm::vec3 pointsBuffer[16];
	glm::vec3 normalBuffer[16];
	float depthsBuffer[16];

	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);             /*****************************************************/
	                                                        /*  Search which polygon of square is this point in  */
	// collision between box vertexes and terrain polygons  /* (0, 0)                   (0, 0)                   */
	for (int i = 0; i < 8; ++i) {                           /*   __________               __________             */
		int ix = (int)std::floor(OBB[i].x);                 /*   |       /|     /    \    |       /|     /    \  */
		int iz = (int)std::floor(OBB[i].z);                 /*   |  (.) / |    |(0, 0)|   |      / |    |(1, 0)| */
		if ((OBB[i].x - ix) + (OBB[i].z - iz) > 1)          /*   |    /   | => |(1, 0)|   |    /   | => |(1, 1)| */
		{                                                   /*   |   /    |    |(0, 1)|   |   /    |    |(0, 1)| */
			ix += 1;                                        /*   | /      |     \    /    | / (.)  |     \    /  */
			iz += 1;                                        /*   |/_______|               |/_______|             */
		}                                                   /*          (1, 1)                   (1, 1)          */
															/*****************************************************/
		glm::vec3 n = terrain->GetTerrainNormal(OBB[i].x, OBB[i].z);
		glm::vec3 a = glm::vec3(ix, terrain->GetPointHeight(ix, iz), iz); // point of polygon
		glm::vec3 o = OBB[i] - glm::dot(OBB[i] - a, n) * n; // projection of OBB[i] on polygon
		float dy = o.y - OBB[i].y;
		if (dy >= 0.0f) // check if OBB[i] is below the polygon
		{	
			pointsBuffer[count] = OBB[i];
			normalBuffer[count] = n;
			depthsBuffer[count] = glm::dot(dy * up, n);
			++count;
		}
	}

	// collision between terrain vertexes and OBB faces
	int x0 = (int)std::floor(std::max(AABB[0].x, 0.0f));
	int z0 = (int)std::floor(std::max(AABB[0].z, 0.0f));
	int x1 = (int)std::ceil(std::min(AABB[1].x, terrain->HeightsMapSize - 1.0f));
	int z1 = (int)std::ceil(std::min(AABB[1].z, terrain->HeightsMapSize - 1.0f));
	for (auto& i : faces)
	{
		glm::vec3 a = OBB[i[0]];
		glm::vec3 b = OBB[i[1]];
		glm::vec3 c = OBB[i[2]];
		glm::vec3 d = OBB[i[3]];

		glm::vec3 n = MathF::Normalize(glm::cross(c - a, b - a));
		if (glm::dot(n, glm::vec3(0.0f, -1.0f, 0.0f)) >= 0.0f)
		{
			continue; // don't check faces which normal directed opposite the terrain 
		}
		// Ax + By + Cz + D = 0 (plane of <a, b, c, d> face)
		float D = -(n.x * a.x + n.y * a.y + n.z * a.z);
		for (int x = x0; x < x1; ++x)
		{
			for (int z = z0; z < z1; ++z)
			{
				float y = -(n.x * x + n.z * z + D) / n.y;
				float terrainY = terrain->GetPointHeight(x, z);
				float dy = terrainY - y;
				if (dy < 0.0f) // check if terrain vertex above the OBB face
				{
					continue;
				}
				glm::vec3 o = glm::vec3(x, terrainY, z);
				// check if an intersection of place is inside OBB face 
				float sum_angle = MathF::GetAngle(a, b, o);
				sum_angle += MathF::GetAngle(b, c, o);
				sum_angle += MathF::GetAngle(c, d, o);
				sum_angle += MathF::GetAngle(d, a, o);

				if (std::abs(2.0f * M_PI - sum_angle) < 0.01f && count < 16)
				{
					pointsBuffer[count] = o;
				    normalBuffer[count] = n;
					depthsBuffer[count] = dy;
					++count;
				}
			}
		}

		if (count >= 16) // don't count too much intersections
		{
			break;
		}
	}

	glm::vec3 n = glm::vec3(0.0f);
	glm::vec3 p = glm::vec3(0.0f);
	depth = 0.0f;
	// count the medium equivalent point and normal
	for (int i = 0; i < count; ++i)
	{
		p += pointsBuffer[i];
		n += normalBuffer[i];
		depth += depthsBuffer[i];
	}
	normal = MathF::Normalize(n);
	point = 1.0f / count * p;
	depth = 1.0f / count * depth;
	//TODO: move to debug class
	Collisions.push_back(point);

	return count > 0;
}
