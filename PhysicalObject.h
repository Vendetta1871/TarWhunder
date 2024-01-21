#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H

#include "PhysicalPoint.h"

class PhysicalObject
{
public:
	float** v_points;
	PhysicalPoint* p_points;

	PhysicalObject(float** vertixes_points, PhysicalPoint* physical_points);
};


#endif
