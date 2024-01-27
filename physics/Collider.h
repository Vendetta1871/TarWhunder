#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>

class Collider
{
private:
	glm::vec3* p;

	void RecalculateBounds();

public:
	unsigned int VertexCount;

	Collider(glm::vec3* vertixes, int n);
	Collider();
	~Collider();

	void RecalculatePoints(glm::vec3 origin, glm::mat3 rot);

	glm::vec3* Points;
	int BoundRect[6];
};

#endif