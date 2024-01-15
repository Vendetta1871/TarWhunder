#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord; 
layout (location = 2) in vec3 v_normal; 

out vec2 a_texCoord;

out vec3 a_fragPos;
out vec3 a_normal;

uniform mat4 model;
uniform mat4 projview;

void main() {
	a_texCoord = v_texCoord;
	
	a_normal = v_normal;
	a_fragPos = vec3(model * vec4(v_position, 1.0));
	
	gl_Position = projview * model * vec4(v_position, 1.0);
}
