#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
	unsigned int id;
public:
	Shader(unsigned int id);
	~Shader();

	void Use();
};

extern Shader* LoadShader(std::string vertexFile, std::string fragmentFile);

#endif
