#version 450

layout(location = 0) in vec3 in_normal;
layout(location = 1) in vec2 in_texCoord;

layout(binding = 0) uniform sampler2D albedo_texture;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;

void main()
{
	vec3 lightDirection = normalize(vec3(-.8, -.7, -.9));
	vec3 albedo = texture(albedo_texture, in_texCoord).rgb;

	float diffuse = max(dot(in_normal, -lightDirection), 0);
	float ambient = .2;

	float light = diffuse + ambient;
	out_color = vec4(albedo * light, 1);
	out_normal = vec4(in_normal, 1);
}