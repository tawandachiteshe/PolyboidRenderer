#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};

out vec3 vTextureCoord;
out vec2 vTextureCoord2D;

struct Vertex {
  float sPosition[3]; 
  float sNormal[3];
  float sUV[2];
  float sTangents[3];
  float sBitagents[3];
  float sMeshIndex;
};


layout(std430, binding = 0) readonly buffer Vertices {
  Vertex in_Vertices[];
};

vec3 getPosition(int i) {
	return vec3( in_Vertices[i].sPosition[0], in_Vertices[i].sPosition[1], in_Vertices[i].sPosition[2]);
}

vec3 getNormal(int i) {
	return vec3( in_Vertices[i].sNormal[0], in_Vertices[i].sNormal[1], in_Vertices[i].sNormal[2]);
}


vec2 getUV(int i) {
	return vec2(in_Vertices[i].sUV[0], in_Vertices[i].sUV[1]);
}




void main() {
	

	//mat4(mat3(view))


	gl_Position = projection * mat4(mat3(view)) * vec4(getPosition(gl_VertexID), 1.0f);

	vTextureCoord = getPosition(gl_VertexID);
	vTextureCoord2D = getUV(gl_VertexID);
}