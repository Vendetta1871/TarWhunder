#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord; 
layout (location = 2) in vec3 v_normal; 

out vec2 a_texCoord;
out float a_light;

uniform mat4 model;
uniform mat4 projview;
uniform vec3 lightPos;

uniform vec3 move;
uniform mat3 rotation;

void main() {
	a_texCoord = v_texCoord;

	vec4 pos = vec4(move + rotation * v_position, 1.0);
	vec3 norm = rotation * v_normal;

	float ambientStrength = 0.2;
	vec3 lightDir = normalize(lightPos - vec3(model * pos));
	float diff = max(dot(norm, lightDir), 0.0);
	a_light = ambientStrength + diff;

	gl_Position = projview * model * pos;
}
