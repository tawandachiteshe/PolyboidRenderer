#version 450 core

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	
};

uniform mat4 uProjection;
uniform mat4 uView;
	

uniform mat4 uTransform;
out vec2 vUV;
out vec3 vNormal;
out vec3 vPixelPosition;
out vec3 vPosition;
out mat3 vTBN;


struct Vertex {

  float sPosition[3]; 
  float sNormal[3];
  float sUV[2];
  float sTangents[3];
  float sBitagents[3];
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

vec3 getTangents(int i) {
	return vec3( in_Vertices[i].sTangents[0], in_Vertices[i].sTangents[1], in_Vertices[i].sTangents[2]);
}


vec3 getBiTangents(int i) {
	return vec3( in_Vertices[i].sBitagents[0], in_Vertices[i].sBitagents[1], in_Vertices[i].sBitagents[2]);
}


void main() {
	

	//mat4(mat3(view))

	vec3 normals = getNormal(gl_VertexID);


	gl_Position = projection * view * uTransform * vec4(getPosition(gl_VertexID), 1.0f);
	vUV = getUV(gl_VertexID);
	vNormal = mat3(transpose(inverse(uTransform))) * normals;
	vPixelPosition = vec3(uTransform * vec4(getPosition(gl_VertexID), 1.0));
	vPosition = getPosition(gl_VertexID);

	vec3 tagents = getTangents(gl_VertexID);
	vec3 bitagents = getBiTangents(gl_VertexID);
	
	vec3 T = normalize(vec3(uTransform * vec4(tagents,   0.0)));
	vec3 B = normalize(vec3(uTransform * vec4(bitagents, 0.0)));
	vec3 N = normalize(vec3(uTransform * vec4(normals,   0.0)));
	mat3 TBN = mat3(T, B, N);

	vTBN = TBN;

}