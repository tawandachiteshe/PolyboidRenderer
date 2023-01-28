#version 450 core

layout(location = 0) out vec4 oAlbedo;
layout(location = 1) out vec4 oNormals;
layout(location = 2) out vec4 oMetalRoughness;
layout(location = 3) out vec4 oPositions;


in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;
in mat3 vTBN;
in flat float vMeshIdx;

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);


struct MaterialData 
{
	float Albedo[3];
	float AO[3];
	float Metallic;
	float Roughness;

	int textures[4];
};

layout(binding = 0) uniform sampler2D albedo;
layout(binding = 1) uniform sampler2D normals;
layout(binding = 2) uniform sampler2D metallic;
layout(binding = 3) uniform sampler2D ao;

struct CameraData 
{
	vec3 ViewPosition;
};


uniform float materialCount;
uniform CameraData uCameraData;

layout(std430, binding = 1) readonly buffer Materials {
  MaterialData in_Materials[];
};

vec3 getAlbedo()
{
	return vec3(in_Materials[int(vMeshIdx)].Albedo[0], in_Materials[int(vMeshIdx)].Albedo[1], in_Materials[int(vMeshIdx)].Albedo[2]);
}


void main()
{
	// Sample input textures to get shading model params.


	vec3 albedo = texture(albedo, vUV).rgb * getAlbedo();
	float metalness = texture(metallic, vUV).b;
	float roughness = texture(metallic, vUV).g;

	if(metalness == 1.0f)
	{
		metalness = in_Materials[int(0)].Metallic;
	}

	if(roughness == 1.0f)
	{
		roughness = in_Materials[int(0)].Roughness;
	}

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(uCameraData.ViewPosition - vPosition);

	// Get current fragment's normal and transform to world space.
	vec3 N = normalize(2.0 * texture(normals, vUV).rgb - 1.0);
	N = normalize(vTBN * N);

	if(texture(normals, vUV).rgb == vec3(1.0))
	{
		N = normalize(vNormal);
	}
	
	vec4 metallicOut = vec4(1.0);
	vec4 normalOut = vec4(N, 1.0);
	metallicOut.b = metalness;
	metallicOut.g = roughness;

	oAlbedo = vec4(albedo, 1.0);
	oMetalRoughness = metallicOut;
	oNormals = normalOut;
	oPositions = vec4(vPixelPosition, 1.0f);

}
