#version 450

layout(location = 0) in vec4 in_normal;
layout(location = 1) in vec2 in_texCoord;

layout(binding = 0) uniform sampler2D tx_diffuse;

layout(location = 0) out vec4 out_color;

void main()
{
    vec3 albedo = texture(tx_diffuse, in_texCoord).rgb;
    float diffuse = max(dot(vec3(1, 0, 0), in_normal.xyz), 0);
    float ambient = .2;
    float light = diffuse + ambient;
    out_color = vec4(albedo * light, 1);
}
