#version 450 core

layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 cameraPosition;
	
};


uniform mat4 uTransform;

out VS_OUT
{
	vec3 vPositionVS;
	vec3 vTangentVS;
	vec3 vBinormalVS;
	vec3 vNormalVS;
	vec2 vUV;
	vec4 vPosition;
	vec3 vPixelPosition;

} vs_out;


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




void main() {
	

	//mat4(mat3(view))

	vec3 normals = getNormal(gl_VertexID);
	vec3 bitangents = getBiTangents(gl_VertexID);
	vec3 tagents = getTangents(gl_VertexID);
	vec2 uv = getUV(gl_VertexID);

	vec4 position =  projection * view * uTransform * vec4(getPosition(gl_VertexID), 1.0f);

	gl_Position = position;
	mat4 viewModel = view * uTransform;

	vs_out.vPositionVS = (viewModel * vec4(getPosition(gl_VertexID), 1.0f)).xyz;
	vs_out.vTangentVS = mat3(viewModel) * tagents;
	vs_out.vNormalVS = mat3(viewModel) * normals;
	vs_out.vBinormalVS = mat3(viewModel) * bitangents;
	vs_out.vUV = uv;
	vs_out.vPosition = position;
	vs_out.vPixelPosition = vec3(uTransform * vec4(getPosition(gl_VertexID), 1.0));

}