#version 450 core

out vec4 FragColor;

in vec4 vColor;
in vec2 vUV;

layout(location = 0) uniform sampler2D uTexture;


void main() {
	
	vec4 tex = texture(uTexture, vUV);

	FragColor = vColor; 

}