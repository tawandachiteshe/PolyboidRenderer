#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aNormal;
layout (location = 2) in vec2 aUV;

out vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;


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



void main()
{
	vPosition = getPosition(gl_VertexID);
	gl_Position = projection * view * vec4(getPosition(gl_VertexID), 1.0);
}

