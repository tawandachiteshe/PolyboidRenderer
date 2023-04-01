#version 450 core



layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aUV;


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 pos;
};


layout(push_constant) uniform uPushConstant {
  float time;
} pc;


layout(location = 0) out VS_OUT {
	vec4 vColor; 
	vec2 vUV;
} vs_out;


void main() {
	
	gl_Position = projection * view * vec4(aPosition, 1.0f);
	vs_out.vColor = aColor;
	vs_out.vUV = aUV;
}