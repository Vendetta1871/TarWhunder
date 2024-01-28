#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

class Shader
{
private:
	unsigned int id;
public:
	Shader(unsigned int id);
	~Shader();

	void Use();
	void UniformMatrix(std::string name, glm::mat4 matrix);
	void UniformMatrix(std::string name, glm::mat3 matrix);
	void UniformVector(std::string name, glm::vec3 vector);
	void UniformFloat(std::string name, const float f);
};

extern Shader* LoadShader(std::string vertexFile, std::string fragmentFile);

#endif
