#version 450

layout(location = 0) in vec2 in_texCoord;

layout(location = 0) out vec4 out_color;

layout(binding = 0) uniform sampler2D position_texture;
layout(binding = 1) uniform sampler2D normal_texture;
layout(binding = 2) uniform sampler2D albedo_texture;

void main()
{
	vec3 albedo = texture(albedo_texture, in_texCoord).rgb;
	// without projection
	vec3 normal = texture(normal_texture, in_texCoord).xyz;

	vec3 lightDirection = normalize(vec3(-.2, -.5, -.2));

	float diffuse = max(dot(normal, -lightDirection), 0);
	float ambient = .2;

	float light = diffuse + ambient;

	out_color = vec4(albedo * light, 1);
}
