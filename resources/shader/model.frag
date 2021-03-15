#version 450

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_texCoord;
layout(location = 2) in vec3 in_position;

layout(binding = 0) uniform sampler2D albedo_texture;

layout(location = 0) out vec4 out_position;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_albedo;

void main()
{
	vec3 albedo = texture(albedo_texture, in_texCoord).rgb;
	out_albedo = vec4(albedo, 1);
	out_normal = vec4(in_normal, 1);
	out_position = vec4(in_position, 1);
}