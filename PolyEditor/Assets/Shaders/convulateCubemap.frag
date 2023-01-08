#version 450 core

out vec4 FragColor;

in vec3 vPosition;

layout (binding = 0) uniform samplerCube skybox;
const float PI = 3.14159265359;

void main() 
{
	vec3 normal = normalize(vPosition);

	vec3 irradiance = vec3(0.0);

	vec3 up = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up, normal);
	up = cross(normal, right);
	float sampleDelta = 0.015;
	float nrSamples = 0.0;


	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta)
	{
		for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta)
		{
		// spherical to cartesian (in tangent space)
			vec3 tangentSample = vec3(sin(theta) * cos(phi), sin(theta) *
			sin(phi), cos(theta));
			// tangent space to world
			vec3 sampleVec = tangentSample.x * right + tangentSample.y * up +
			tangentSample.z * normal;
			irradiance += texture(skybox, -sampleVec).rgb * cos(theta) *
			sin(theta);
			nrSamples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));



	FragColor = vec4(irradiance, 1.0);
}