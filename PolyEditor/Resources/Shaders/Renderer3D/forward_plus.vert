#version 460


layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;


layout (location = 0) out VS_OUT {

	vec4 vNormal;
	vec2 vUV;

} vs_out;

layout (std140, set = 0, binding = 0) uniform PerSceneData {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};

layout(push_constant) uniform PerObject {

	mat4 transform;
	mat4 padding;

};


void main() {

	gl_Position = projection * view * transform * vec4(aPosition, 1.0);
	vs_out.vUV = aUV;
	vs_out.vNormal = aNormal;
}