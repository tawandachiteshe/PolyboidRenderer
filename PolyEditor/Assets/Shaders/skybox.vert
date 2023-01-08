#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};

out vec3 vTextureCoord;

void main() {
	

	//mat4(mat3(view))


	gl_Position = projection * mat4(mat3(view)) * vec4(aPosition, 1.0f);

	vTextureCoord = aPosition;
}