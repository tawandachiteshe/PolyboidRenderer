#version 450 core

in vec2 vUV;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D uTexture;

float near = 0.1f;
float far = 2000.0f;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}


void main()
{
	vec2 uv = vUV;

	vec4 color = texture(uTexture, uv);

	FragColor = vec4(color.xxx, 1.0f);
}