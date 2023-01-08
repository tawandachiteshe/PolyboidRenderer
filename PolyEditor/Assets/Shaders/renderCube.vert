#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	vPosition = aPosition;
	gl_Position = projection * view * vec4(aPosition, 1.0);
}

