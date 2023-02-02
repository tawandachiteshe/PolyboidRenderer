#version 450 core



layout (location = 0) in vec3 aWorldPosition;
layout (location = 1) in vec3 aLocalPosition;
layout (location = 2) in vec4 aColor;
layout (location = 3) in float aThickness;
layout (location = 4) in float aFade;


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 pos;
	
};


out vec3 vLocalPosition;
out vec4 vColor;
out float vThickness;
out float vFade;


void main() {
	
	gl_Position = projection * view * vec4(aWorldPosition, 1.0f);
	
	vLocalPosition = aLocalPosition;
	vColor = aColor;
	vThickness = aThickness;
	vFade = aFade;

}