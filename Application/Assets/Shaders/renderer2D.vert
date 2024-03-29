#version 450 core



layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;

uniform mat4 uViewProj;
uniform mat4 uTransform;

layout (std140, binding = 0) uniform CameraData {
    
    uniform mat4 camera;

};


out vec4 vColor; 

void main() {
	
	gl_Position = camera * vec4(aPosition, 1.0f);
	vColor = aColor;

}