#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec2 v_texCoord; 
layout (location = 2) in float v_light; 

out vec2 a_texCoord;
out float a_light;

uniform mat4 model;
uniform mat4 projview;

void main() {
	a_texCoord = v_texCoord;
	a_light = v_light;
	
	gl_Position = projview * model * vec4(v_position, 1.0);
}
