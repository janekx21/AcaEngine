#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoords;

layout(location = 0) out vec2 out_texCoords;

void main()
{
	gl_Position = vec4(in_position, 1);
	out_texCoords = in_texCoords;
}