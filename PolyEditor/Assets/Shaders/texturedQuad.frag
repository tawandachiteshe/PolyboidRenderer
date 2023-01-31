#version 450 core

in vec2 vUV;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D uTexture;


void main()
{
	vec2 uv = vUV;

	vec4 color = texture(uTexture, uv);

	FragColor = vec4(color.xyz, 1.0f);
}