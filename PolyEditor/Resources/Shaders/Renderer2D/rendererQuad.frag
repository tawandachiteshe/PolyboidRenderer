#version 450 core

layout(location = 0) out vec4 FragColor;



layout(location = 0) in VS_OUT {
	vec4 vColor;
	vec2 vUV;
} vs_in; 

//layout(binding = 0) uniform sampler2D uTexture;


void main() {
	
	//vec4 tex = texture(uTexture, vs_in.vUV);

	FragColor = vs_in.vColor; 

}