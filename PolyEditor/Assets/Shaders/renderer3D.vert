#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};


uniform mat4 uTransform;
out vec2 vUV;
out vec3 vNormal;
out vec3 vPixelPosition;
out vec3 vPosition;

void main() {
	

	//mat4(mat3(view))

	gl_Position = projection * view * uTransform * vec4(aPosition, 1.0f);
	vUV = aUV;
	vNormal = mat3(transpose(inverse(uTransform))) * aNormal;
	vPixelPosition = vec3(uTransform * vec4(aPosition, 1.0));
	vPosition = aPosition;

}