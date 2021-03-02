#version 450

layout(location = 0) in vec2 in_texCoord;

layout(binding = 0) uniform sampler2D color_texture;
layout(binding = 1) uniform sampler2D depth_texture;
layout(binding = 2) uniform sampler2D normal_texture;

layout(location = 0) out vec4 out_color;

void main()
{
	vec3 albedo = texture(color_texture, in_texCoord).rgb;
	float depth = texture(depth_texture, in_texCoord).r;
	vec3 normal = texture(normal_texture, in_texCoord).xyz;
	float light = 1 - depth;
	out_color = vec4(normal, 1);
}