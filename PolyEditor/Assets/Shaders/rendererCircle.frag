#version 450 core

out vec4 oColor;

in vec3 vLocalPosition;
in vec4 vColor;
in float vThickness;
in float vFade;


void main() {



	float distance = 1.0 - length(vLocalPosition);
    float circle = smoothstep(0.0, vFade, distance);
    circle *= smoothstep(vThickness, vThickness + vFade, distance);

	if (circle <= 0.0)
		discard;

    // Set output color


	oColor = vColor; 
	oColor.a *= circle;

}