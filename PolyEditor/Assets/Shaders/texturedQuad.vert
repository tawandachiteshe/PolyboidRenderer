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

out vec2 vUV;

void main()
{
	
	gl_Position = projection * view * vec4(aPosition, 1.0);

	vUV = aUV;
}

