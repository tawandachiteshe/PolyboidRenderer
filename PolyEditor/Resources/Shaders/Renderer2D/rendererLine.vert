#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 pos;
	
};


layout(location = 0) out VS_OUT 
{
	vec4 vColor;
} vs_out;


void main() {
	
	gl_Position = projection * view * vec4(aPosition, 1.0f);
	vs_out.vColor = aColor;
}