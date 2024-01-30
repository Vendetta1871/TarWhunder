#ifndef COLLIDER_H
#define COLLIDER_H

#include <vector>

#include <glm/glm.hpp>

class Terrain;
/*==========
Box Collider
==========*/
class Collider
{
private:
	glm::vec3* vertexes; // collider vertexes positions in local coordinates
	glm::vec3* AABB; // extreme two points of bounding based-vectors oriented cuboid
	
	void UpdateAABB();
	// return index of furthest point of OBB in <direction> direction
	int GetFurthestPointIndex(const glm::vec3& direction) const;
	/*=====================================================
	Check if <o>-<m> line segment intersects a face of OBB.
	params:
	o[in] - first point of line segment
	m[in] - second point of line segment
	point[out] - point of intersection with face
	=====================================================*/
	bool IntersectLineOBB(const glm::vec3& o, const glm::vec3& m, glm::vec3& point) const;
	/*=======================================
	Return point of the contact of colliders.
	params:
	obj[in] - colliding object
	n[in] - collision normal vector
	=======================================*/
	glm::vec3 GetContactPoint(const Collider* obj, const glm::vec3& n) const;
public:
	glm::vec3* OBB; // collider vertexes positions in world coordinates

	Collider(float a, float b, float c);
	~Collider();
	// recalculate OBB vertexes coordinates
	void UpdateOBB(const glm::vec3& position, const glm::mat3& rotation);	
	/*================================================
	Separating Axis Theorem based collision detection.
	params:
	obj[in] - other collider to check collision
	normal[out] - collision normal (directed inside <obj> body)
	point[out] - point of contact
	depth[out] - depth of intersection
	================================================*/
	bool SAT(const Collider* obj, glm::vec3& normal, glm::vec3& point, float& depth) const;
	/*===================================
	Terrain-Collider collision detection.
	params:
	terrain[in] - terrain
	normal[out] - collision normal (directed inside <this> body)
	point[out] - point of contact
	depth[out] - depth of intersection
	===================================*/
	bool IsTerrainCollision(const Terrain* terrain, glm::vec3& normal, glm::vec3& point, float& depth) const;

	//TODO: move to Debug class
	static std::vector<glm::vec3> Collisions;
};

#endif
