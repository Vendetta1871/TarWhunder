#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord; 
layout (location = 2) in vec3 v_normal; 

out vec2 a_texCoord;
out float a_light;

uniform mat4 model;
uniform mat4 projview;
uniform vec3 lightPos;

void main() {
	a_texCoord = v_texCoord;

	float ambientStrength = 0.2;
	vec3 lightDir = normalize(lightPos - vec3(model * vec4(v_position, 1.0)));
	float diff = max(dot(v_normal, lightDir), 0.0);
	a_light = ambientStrength + diff;
	
	gl_Position = projview * model * vec4(v_position, 1.0);
}
