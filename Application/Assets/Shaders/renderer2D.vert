#version 450 core



layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 uViewProj;
uniform mat4 uTransform;

out vec4 vColor; 

void main() {
	
	gl_Position = uViewProj * vec4(aPosition, 1.0f);
	vColor = aColor;

}