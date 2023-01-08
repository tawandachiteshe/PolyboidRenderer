#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};


uniform mat4 uLightProjection;
uniform mat4 uLightView;
uniform mat4 uTransform;


void main()
{
	gl_Position = uLightProjection * uLightView * uTransform * vec4(aPosition, 1.0);
}

