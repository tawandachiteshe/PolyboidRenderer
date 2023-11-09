#version 460


layout (location = 0) in VS_OUT {

	vec4 vNormal;
	vec2 vUV;

} vs_in;

layout (location = 0) out vec4 oColor;




layout (std140, set = 1, binding = 0) uniform Material {
    
	vec3 Color;
};

/*
layout (std140, set = 0, binding = 2) uniform MaterialData {
    
	uniform Material material;
	
};
*/



void main() {

	oColor = vec4(Color, 1);//vec4(material.Color, 1);
	
}