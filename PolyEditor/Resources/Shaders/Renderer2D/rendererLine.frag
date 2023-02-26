#version 450 core

layout(location = 0) out vec4 oColor;
layout(location = 0) uniform vec2 a;


layout(location = 0) in VS_OUT 
{
	vec4 vColor;
} vs_in;

void main() {
	
	oColor = vs_in.vColor; 

}