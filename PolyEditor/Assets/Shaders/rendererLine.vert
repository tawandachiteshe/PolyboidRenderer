#version 450 core

layout (location = 0) in vec3 aWorldPosition;
layout (location = 1) in vec3 aLocalPosition0;
layout (location = 2) in vec3 aLocalPosition1;
layout (location = 3) in vec4 aColor;
layout (location = 4) in float aThickness;
layout (location = 5) in vec2 aUV;


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};


out vec3 vLocalPosition0;
out vec3 vLocalPosition1;
out vec4 vColor;
out float vThickness;
out vec2 vUV;


void main() {
	
	gl_Position = projection * view * vec4(aWorldPosition, 1.0f);
	
	vColor = aColor;
	vThickness = aThickness;
	vLocalPosition0 = aLocalPosition0;
	vLocalPosition1 = aLocalPosition1;
	vThickness = aThickness;
	vUV = aUV;

}