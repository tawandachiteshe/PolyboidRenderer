#ifndef COMMON_VERT
#define COMMON_VERT

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


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 cameraPosition;
	uniform mat4 transform;
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

vec3 getTangents(int i) {
	return vec3( in_Vertices[i].sTangents[0], in_Vertices[i].sTangents[1], in_Vertices[i].sTangents[2]);
}


vec3 getBiTangents(int i) {
	return vec3( in_Vertices[i].sBitagents[0], in_Vertices[i].sBitagents[1], in_Vertices[i].sBitagents[2]);
}

float getMeshID(int i)
{
	return in_Vertices[i].sMeshIndex;
}

#endif //Common vert