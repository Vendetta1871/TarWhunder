#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>
/*===================
Shader handling class
===================*/
class Shader
{
private:
	unsigned int id; // openGL shader program ID
public:
	Shader(unsigned int id);
	~Shader();
	// set shader active
	void Use();
	// set shader <matrix> mat4 uniform
	void SetUniformMatrix(const std::string& name, const glm::mat4& matrix);
	// set shader <matrix> mat3 uniform
	void SetUniformMatrix(const std::string& name, const glm::mat3& matrix);
	// set shader <vector> vec3 uniform
	void SetUniformVector(const std::string& name, const glm::vec3& vector);
};
//TODO: move to loader class
// load vertex and fragment shaders and create <Shader*> object 
extern Shader* LoadShader(const std::string& vertex_filename, const std::string& fragment_filename);

#endif
