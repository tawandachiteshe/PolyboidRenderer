#version 450 core

out vec4 oColor;

in vec4 vColor;
in vec3 vLocalPosition0;
in vec3 vLocalPosition1;
in float vThickness;
in vec2 vUV;

void main() {

	vec2 p0 = vLocalPosition0.xy;
	vec2 p1 = vLocalPosition1.xy;
	vec2 p3 = (vUV * 2.0) - vec2(1.0);
	p3 *= length(p1 - p0);
	vec2 p12 = p1 - p0;
	vec2 p13 = p3 - p0;

	float d = dot(p12, p13) / length(p12);
	vec2 p4 = p0 + normalize(p12) * d;

	vec4 color = vColor;


	if(length(p4 - p3) <= vThickness
		&& length(p4 - p0)
		<= length(p12)
		&& length(p4 - p1)
		<= length(p12)
	)
	{
		
	} 
	else
	{
		discard;
	}

	

	oColor = vec4(color);

}