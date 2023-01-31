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


layout(binding = 0) uniform sampler2D uTextures[28];
struct CameraData 
{
	vec3 ViewPosition;
};

uniform float materialCount;
uniform CameraData uCameraData;
uniform int uMaterialIndex = 0;

layout(std430, binding = 1) readonly buffer Materials {
  MaterialData in_Materials[];
};



vec3 getAlbedo()
{
	return vec3(in_Materials[uMaterialIndex].Albedo[0], in_Materials[uMaterialIndex].Albedo[1], in_Materials[uMaterialIndex].Albedo[2]);
}

int getTextureIndex(int textureIdx)
{

	return in_Materials[uMaterialIndex].textures[textureIdx];
}

vec4 getTexture(int textureIdx)
{
	switch(getTextureIndex(textureIdx))
	{
		case 0:
			return texture(uTextures[0], vUV);
		case 1:
			return texture(uTextures[1], vUV);
		case 2:
			return texture(uTextures[2], vUV);
		case 3:
			return texture(uTextures[3], vUV);
		case 4:
			return texture(uTextures[4], vUV);
	}

	return vec4(0.54, 0.98, 0.67, 1.0);
}

vec3 getAlbedoTexture()
{
	if(getTextureIndex(0) == -1)
	{
		return getAlbedo();
	} else {
	
		return getTexture(0).rgb;
	}
}

vec3 getNormalTexture()
{
	if(getTextureIndex(1) == -1)
	{
		return normalize(vNormal);
	} else {
	
		vec3 N = normalize(2.0 * getTexture(1).rgb - 1.0);
		return normalize(vTBN * N);
	}
}

vec3 getMetalRougnessTexture()
{
	if(getTextureIndex(2) == -1)
	{
		return vec3(1.0, 1.0, 1.0);
	} else {
	
		return getTexture(2).rgb;
	}
}


void main()
{
	// Sample input textures to get shading model params.
	vec3 albedo = getAlbedoTexture();
	vec3 metalnessRoughness = getMetalRougnessTexture();

	float metalness = metalnessRoughness.b;
	float roughness = metalnessRoughness.g;

	if(metalness == 1.0f)
	{
		metalness = in_Materials[uMaterialIndex].Metallic;
	}

	if(roughness == 1.0f)
	{
		roughness = in_Materials[uMaterialIndex].Roughness;
	}

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(uCameraData.ViewPosition - vPosition);

	// Get current fragment's normal and transform to world space.
	vec3 N = getNormalTexture();
	
	vec4 metallicOut = vec4(1.0);
	vec4 normalOut = vec4(N, 1.0);
	metallicOut.b = metalness;
	metallicOut.g = roughness;

	oAlbedo = vec4(albedo, 1.0);
	oMetalRoughness = metallicOut;
	oNormals = normalOut;
	oPositions = vec4(vPixelPosition, 1.0f);

}
