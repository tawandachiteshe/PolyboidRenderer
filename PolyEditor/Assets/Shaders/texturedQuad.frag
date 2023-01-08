#version 450 core

layout (location = 0) out vec4 FragColor;

layout (location = 0) uniform sampler2D uShadowMap;
layout (location = 1) uniform sampler2D uShadowMap2;

in vec2 vUV;

void main()
{
	vec2 uv = vUV;

	//uv.x = -uv.x;
	vec3 shadowMap = texture2D(uShadowMap, uv).xyz;
	vec3 shadowMap2 = texture2D(uShadowMap, uv).xxx;
	FragColor = vec4(shadowMap ,1.0f);
}