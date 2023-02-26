#version 450 core


layout (location = 0) out vec4 FragColor;
layout (location = 0) in vec3 vPosition;
layout (binding = 0) uniform sampler2D hdr;
const vec2 invAtan = vec2(0.1591, 0.3183);


vec2 SampleSphericalMap(vec3 v)
{
	vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
	uv *= invAtan;
	uv += 0.5;
	return uv;
}


void main()
{
	vec2 uv = SampleSphericalMap(normalize(vPosition)); // normalize
	vec3 color = texture(hdr, uv).rgb;	
	FragColor = vec4(color, 1.0);
}
