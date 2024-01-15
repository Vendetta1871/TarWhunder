#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture
{
	unsigned int id;
	int width;
	int height;
public:
	Texture(unsigned int id, int width, int height);
	~Texture();

	void Bind();
};

extern Texture* LoadTexture(std::string filename);

#endif
