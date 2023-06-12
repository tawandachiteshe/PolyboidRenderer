#version 460

layout(location = 0) out vec4 FragColor;



layout(location = 0) in vec2 vTextureCoord2D;
layout(location = 2) flat in uint vAge;


layout (binding = 2) uniform sampler2D skybox;
layout (binding = 4) uniform samplerCube skyboxCube;


const float PI = 3.14159265359;

void main() {

	//vec3 envColor = textureLod(skybox, -vTextureCoord, 1.2).xyz;
	
	vec3 envSampler = textureLod(skybox, vTextureCoord2D, 0).xyz;

	//envColor = envColor / (envColor + vec3(1.0));
	//envColor = pow(envColor, vec3(1.0/2.2));
	//FragColor = vec4(envColor, 1.0);


	
	FragColor = vec4(envSampler, 255.0f);

}