#version 450 core

layout (location = 0) out vec4 oColor;

layout (location = 0) in VS_OUT 
{
	vec3 vLocalPosition;
	vec4 vColor;
	float vThickness;
	float vFade;
} vs_in;



void main() {



	float distance = 1.0 - length(vs_in.vLocalPosition);
    float circle = smoothstep(0.0, vs_in.vFade, distance);
    circle *= smoothstep(vs_in.vThickness, vs_in.vThickness + vs_in.vFade, distance);

	if (circle <= 0.0)
		discard;

    // Set output color


	oColor = vs_in.vColor; 
	oColor.a *= circle;

}