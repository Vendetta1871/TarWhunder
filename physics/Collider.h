#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <vector>

class Collider
{
private:
	glm::vec3* p;

	void RecalculateBounds();

	int _getFurthestPointIndex(glm::vec3 d);

public:
	unsigned int VertexCount;

	Collider(glm::vec3* vertixes, int n);
	Collider();
	~Collider();

	void RecalculatePoints(glm::vec3 origin, glm::mat3 rot);

	glm::vec3* Points;
	float BoundRect[6];

	std::pair<glm::vec3, glm::vec3> GetCollisionPoints(float** h_map, int h_map_size, float& depth);

	bool SAT_OBBOBB(glm::vec3* obj, glm::vec3& normal, float& depth);
	glm::vec3 GetCollisionPoint_OBBOBB(Collider* obj, glm::vec3 n);

	static std::vector<glm::vec3> Collisions;
};

#endif