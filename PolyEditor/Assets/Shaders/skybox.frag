#version 450 core

out vec4 FragColor;

in vec3 vTextureCoord;

layout (binding = 0) uniform samplerCube skybox;

const float PI = 3.14159265359;

void main() {

	vec3 envColor = textureLod(skybox, -vTextureCoord, 1.2).xyz;
	

	//envColor = envColor / (envColor + vec3(1.0));
	//envColor = pow(envColor, vec3(1.0/2.2));
	//FragColor = vec4(envColor, 1.0);

	
	FragColor = vec4(envColor, 1.0f);

}