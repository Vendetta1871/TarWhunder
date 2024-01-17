#ifndef MATH_H
#define MATH_H

class Math
{
public:
	static void GetVertexNormal(float** h_map, int size, int x, int z, float* n_x, float* n_y, float* n_z);
	static void DiamondSquare(float** h_map, int size, int step, int depth, int scale, float f);
	static float** SmoothArray(float** array, int size);
};

#endif
