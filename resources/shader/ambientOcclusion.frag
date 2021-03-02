#version 450

#define KERNEL_SIZE 64

layout(location = 0) in vec2 in_texCoord;

layout(binding = 0) uniform sampler2D color_texture;
layout(binding = 1) uniform sampler2D depth_texture;
layout(binding = 2) uniform sampler2D normal_texture;
layout(binding = 3) uniform sampler2D position_texture;
layout(binding = 4) uniform sampler2D noise_texture;
uniform vec3 samples[KERNEL_SIZE];
uniform mat4 projection_matrix;

layout(location = 0) out vec4 out_color;

// TODO replace with uniform
const vec2 noiseScale = vec2(1366.0/8.0, 768.0/8.0);

void main()
{
	vec3 albedo = texture(color_texture, in_texCoord).rgb;
	vec3 normal = texture(normal_texture, in_texCoord).xyz;
	vec3 position = texture(position_texture, in_texCoord).xyz;
	vec3 randomVector = texture(noise_texture, in_texCoord * noiseScale).xyz;

	vec3 tangent = normalize(randomVector - normal * dot(randomVector, normal));
	vec3 bitangent = cross(normal, tangent);
	mat3 TBN = mat3(tangent, bitangent, normal);

	float bias = .025;
	float radius = 2;

	float occlusion = 0.0;
	for(int i = 0; i < KERNEL_SIZE; i++)
	{
		// get sample position
		vec3 s = samples[i];
		vec3 samplePosition = TBN * s;// from tangent to view-space
		samplePosition = position + samplePosition * radius;

		vec4 offset = vec4(samplePosition, 1.0);
		offset = projection_matrix * offset;    // from view to clip-space
		offset.xyz /= offset.w;               // perspective divide
		offset.xyz = offset.xyz * 0.5 + 0.5; // transform to range 0.0 - 1.0

		float sampleDepth = texture(position_texture, offset.xy).z;
		occlusion += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0);

		// float rangeCheck = smoothstep(0.0, 1.0, radius / abs(position.z - sampleDepth));
		// occlusion += (sampleDepth >= samplePosition.z + bias ? 1.0 : 0.0) * rangeCheck;
	}

	float ao = 1.0 - (occlusion / KERNEL_SIZE);
	out_color = vec4(albedo * mix(1, ao, .6), 1);
	// out_color = vec4(TBN * vec3(0,0,1), 1);
	// out_color = vec4(vec3(position), 1);
	// out_color = vec4(randomVector * .5 + .5, 1);
	// out_color = vec4(normal, 1);
}