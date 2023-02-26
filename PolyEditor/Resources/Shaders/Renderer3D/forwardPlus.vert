#version 450 core

#include "Common/Common.vert.glsl"

layout(location = 0) out VS_OUT
{
	vec3 vTangentVS;
	vec3 vBinormalVS;
	vec3 vNormalVS;
	vec2 vUV;
	vec3 vPositionWS;
	vec4 vPositionVS;
	mat4 model;

} vs_out;

void main() {
	
	
	//mat4(mat3(view))

	vec3 normals = getNormal(gl_VertexID);
	vec3 bitangents = getBiTangents(gl_VertexID);
	vec3 tagents = getTangents(gl_VertexID);
	vec2 uv = getUV(gl_VertexID);

	vec4 position =  projection * view * transform * vec4(getPosition(gl_VertexID), 1.0f);

	mat4 Model = transform;
	vs_out.vTangentVS = normalize(mat3(view * Model) * tagents);
	vs_out.vNormalVS = normalize(mat3(view * Model) * normals);
	vs_out.vBinormalVS = normalize(mat3(view * Model) * bitangents);
	vs_out.vUV = uv;
	vs_out.model = Model;
	vs_out.vPositionWS = vec3(Model * vec4(getPosition(gl_VertexID), 1.0));
	vs_out.vPositionVS = view * Model * vec4(getPosition(gl_VertexID), 1.0);

	gl_Position = position;

}