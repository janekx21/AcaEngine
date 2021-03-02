#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texCoords;

layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_texCoords;
layout(location = 2) out vec4 out_position;

layout(location = 0) uniform mat4 mvp_matrix;
layout(location = 1) uniform mat4 model_matrix;
layout(location = 2) uniform mat4 view_matrix;

void main()
{
	gl_Position = mvp_matrix * vec4(in_position, 1);
	// TODO is this legid
	// out_normal = (model_matrix * vec4(in_normal, 0)).xyz;
	out_normal = (view_matrix * vec4(in_normal, 0)).xyz;
	out_texCoords = in_texCoords;
	out_position = view_matrix * vec4(in_position, 1);
}