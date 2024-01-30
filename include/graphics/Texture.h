#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

typedef unsigned int uint;
/*====================
Texture handling class
====================*/
class Texture
{
	uint id; // openGL texture ID
	int width;
	int height;
public:
	Texture(uint id, int width, int height);
	~Texture();
	// load texture to GPU
	void Bind();
};
//TODO: move to a loader class
// load texture from <filename> image
extern Texture* LoadTexture(const std::string& filename);

#endif
