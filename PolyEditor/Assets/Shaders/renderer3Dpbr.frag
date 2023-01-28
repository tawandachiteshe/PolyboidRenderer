#version 450 core

out vec4 FragColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;
in mat3 vTBN;

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);


struct CameraData 
{
	vec3 ViewPosition;
};


struct DirectionalLight
{
	float Direction[3];
	float Color[3];
	float Energy;
};

struct PointLight
{
	float Position[3];
	float Color[3];
	float Distance;
	float Energy;
};


struct SpotLight
{
	float Position[3];
	float Direction[3];
	float Color[3];
	float InnerAngle;
	float OuterAngle;
	float Distance;
	float Energy;
	
};

struct MaterialData
{
	float Albedo[3];
	float AO[3];
	float Metallic;
	float Roughness;
	int textures[4];

};



uniform int uPointLightsCount;
uniform int uSpotLightsCount;
uniform int uDirectionLightsCount;
uniform int uMaterialIndex = 0;


layout(binding = 0) uniform sampler2D uTextures[28];
layout (binding = 28) uniform samplerCube uPrefilterTex;
layout (binding = 29) uniform sampler2D uBrdfLUTtex;
layout (binding = 30) uniform samplerCube uIrradianceTex;
layout (binding = 31) uniform samplerCube uEnvTex;

layout(std430, binding = 1) readonly buffer Materials {
  MaterialData in_Materials[];
};

layout(std430, binding = 2) readonly buffer PointLights {
  PointLight in_PointLights[];
};


layout(std430, binding = 3) readonly buffer SpotLights {
  SpotLight in_SpotLights[];
};

layout(std430, binding = 4) readonly buffer DirLights {
  DirectionalLight in_DirLights[];
};


uniform CameraData uCameraData;

vec3 float3ToVec3(float p[3])
{
	return vec3(p[0], p[1], p[2]);
}


float CalculateAttenuation(float Range, float Distance)
{
	float Linear = 0.5;
	float Quadratic = 0.5;
	
	float r = Range;
	float I =  (Distance / (Distance + Linear * r)) * (Distance * Distance / ((Distance * Distance) + Quadratic * (r * r)));
	float Sphere = I * (Distance - r) / Distance * float(r < Distance);
	return Sphere;
}


float CalculateAttenuationV2(float Range, float Distance)
{
	return 1.0 - CalculateAttenuation(Range, Distance);
}

float SpotLightDistance(int lightIndex)
{
	return length(float3ToVec3(in_SpotLights[lightIndex].Position) - vPixelPosition);
}

vec3 SpotLightL(int lightIndex)
{
	
	vec3 L = float3ToVec3(in_SpotLights[lightIndex].Position) - vPixelPosition;
	float Distance = SpotLightDistance(lightIndex);
	L = L / Distance;
	return L;
}

float CalculateSpotCone(int lightIndex)
{
	// If the cosine angle of the light's direction
	// vector and the vector from the light source to the point being
	// shaded is less than minCos, then the spotlight contribution will be 0.

	SpotLight light = in_SpotLights[lightIndex];

	float minCos = cos( radians( light.InnerAngle ) );
	// If the cosine angle of the light's direction vector
	// and the vector from the light source to the point being shaded
	// is greater than maxCos, then the spotlight contribution will be 1.
	float maxCos = mix( minCos, 1.0, 0.5f );

	float cosAngle = dot( float3ToVec3(light.Direction), -SpotLightL(lightIndex) );
	// Blend between the minimum and maximum cosine angles.
	return smoothstep( minCos, maxCos, cosAngle );
}

vec3 SpotLightColor(int lightIndex)
{
	return float3ToVec3(in_SpotLights[lightIndex].Color); 
}

vec3 SpotRadiance(int lightIndex)
{
	float spotIntensity = CalculateSpotCone(lightIndex);

	float attenuation = CalculateAttenuationV2(in_SpotLights[lightIndex].Distance, SpotLightDistance(lightIndex)) * in_SpotLights[lightIndex].Energy * spotIntensity;

	return SpotLightColor(lightIndex) * attenuation;
}

vec3 UnpackNormals(vec3 normals)
{
	return normals * 2.0f - 1.0f;
}

float PointLightDistance(int lightIndex)
{
	return length(float3ToVec3(in_PointLights[lightIndex].Position) - vPixelPosition);
}

vec3 PointLightL(int lightIndex)
{
	vec3 L = float3ToVec3(in_PointLights[lightIndex].Position) - vPixelPosition;
	float Distance = PointLightDistance(lightIndex);
	L = L / Distance;
	return L;
}

vec3 PointLightColor(int lightIndex)
{
	return float3ToVec3(in_PointLights[lightIndex].Color);
}

vec3 PointLightRadiance(int lightIndex)
{
	
	float attenuation = CalculateAttenuationV2(in_PointLights[lightIndex].Distance, PointLightDistance(lightIndex));
	
	return PointLightColor(lightIndex) * attenuation * in_PointLights[lightIndex].Energy;
}


vec3 DirectionalLightL(int lightIndex)
{
	vec3 L = normalize(float3ToVec3(in_DirLights[lightIndex].Direction));
	return L;
}

vec3 DirLightColor(int lightIndex)
{
	return float3ToVec3(in_DirLights[lightIndex].Color);
}

vec3 DirLightRadiance(int lightIndex)
{
	return DirLightColor(lightIndex) * in_DirLights[lightIndex].Energy;
}


// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float ndfGGX(float cosLh, float roughness)
{
	float alpha   = roughness * roughness;
	float alphaSq = alpha * alpha;

	float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
	return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float gaSchlickG1(float cosTheta, float k)
{
	return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
	return gaSchlickG1(cosLi, k) * gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
	return F0 + (vec3(1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}


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
		case 5:
			return texture(uTextures[5], vUV);
		case 6:
			return texture(uTextures[6], vUV);
		case 7:
			return texture(uTextures[7], vUV);
		case 8:
			return texture(uTextures[8], vUV);
		case 9:
			return texture(uTextures[0], vUV);
		case 10:
			return texture(uTextures[10], vUV);
		case 11:
			return texture(uTextures[11], vUV);
		case 12:
			return texture(uTextures[12], vUV);
		case 13:
			return texture(uTextures[13], vUV);
		case 14:
			return texture(uTextures[14], vUV);
		case 15:
			return texture(uTextures[15], vUV);
		case 16:
			return texture(uTextures[16], vUV);
		case 17:
			return texture(uTextures[17], vUV);
		case 18:
			return texture(uTextures[18], vUV);
		case 19:
			return texture(uTextures[19], vUV);
		case 20:
			return texture(uTextures[20], vUV);
		case 21:
			return texture(uTextures[21], vUV);
		case 22:
			return texture(uTextures[22], vUV);
		case 23:
			return texture(uTextures[23], vUV);
		case 24:
			return texture(uTextures[24], vUV);
		case 25:
			return texture(uTextures[25], vUV);
		case 26:
			return texture(uTextures[26], vUV);
		case 27:
			return texture(uTextures[27], vUV);
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

struct SurfaceData 
{
	vec3 Lo;
	vec3 N;
	vec3 F0;
	float roughness;
	float cosLo;
	float metalness;
	vec3 albedo;
};

vec3 CalculateDirectLight(vec3 Li, vec3 Lradiance, SurfaceData data)
{


	// Half-vector between Li and Lo.
	vec3 Lh = normalize(Li + data.Lo);

	// Calculate angles between surface normal and various light vectors.
	float cosLi = max(0.0, dot(data.N, Li));
	float cosLh = max(0.0, dot(data.N, Lh));

	// Calculate Fresnel term for direct lighting. 
	vec3 F  = fresnelSchlick(data.F0, max(0.0, dot(Lh, data.Lo)));
	// Calculate normal distribution for specular BRDF.
	float D = ndfGGX(cosLh, data.roughness);
	// Calculate geometric attenuation for specular BRDF.
	float G = gaSchlickGGX(cosLi, data.cosLo, data.roughness);

	// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
	// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
	// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
	vec3 kd = mix(vec3(1.0) - F, vec3(0.0), data.metalness);

	// Lambert diffuse BRDF.
	// We don't scale by 1/PI for lighting & material units to be more convenient.
	// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
	vec3 diffuseBRDF = kd * data.albedo;

	// Cook-Torrance specular microfacet BRDF.
	vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * cosLi * data.cosLo);

	// Total contribution for this light.
	return (diffuseBRDF + specularBRDF) * Lradiance * cosLi;

}


void main()
{
	// Sample input textures to get shading model params.
	SurfaceData data;
	vec3 albedo = getAlbedoTexture();
	data.albedo = albedo;

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

	data.roughness = roughness;
	data.metalness = metalness;

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(uCameraData.ViewPosition - vPosition);
	data.Lo = Lo;

	// Get current fragment's normal and transform to world space.
	vec3 N = getNormalTexture();
	data.N = N;
	
	// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));
	data.cosLo = cosLo;
		
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	vec3 F0 = mix(Fdielectric, albedo, metalness);
	data.F0 = F0;

	// Direct lighting calculation for analytical lights.
	vec3 directLighting = vec3(0);

	//Point Lights
	for(int i = 0; i < uPointLightsCount; ++i)
	{
		vec3 Li = PointLightL(i);
		vec3 Lradiance = PointLightRadiance(i);

		directLighting += CalculateDirectLight(Li, Lradiance, data);
	}

	//Spot Lights
	for(int i = 0; i < uSpotLightsCount; ++i)
	{
		vec3 Li = SpotLightL(i);
		vec3 Lradiance = SpotRadiance(i);

		directLighting += CalculateDirectLight(Li, Lradiance, data);
	}

	//Dir Lights
	for(int i = 0; i < uDirectionLightsCount; ++i)
	{
		vec3 Li = DirectionalLightL(i);
		vec3 Lradiance = DirLightRadiance(i);

		directLighting += CalculateDirectLight(Li, Lradiance, data);
	}


	// Ambient lighting (IBL).
	vec3 ambientLighting;
	{
		// Sample diffuse irradiance at normal direction.
		vec3 irradiance = texture(uIrradianceTex, N).rgb;

		// Calculate Fresnel term for ambient lighting.
		// Since we use pre-filtered cubemap(s) and irradiance is coming from many directions
		// use cosLo instead of angle with light's half-vector (cosLh above).
		// See: https://seblagarde.wordpress.com/2011/08/17/hello-world/
		vec3 F = fresnelSchlick(F0, cosLo);

		// Get diffuse contribution factor (as with direct lighting).
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

		// Irradiance map contains exitant radiance assuming Lambertian BRDF, no need to scale by 1/PI here either.
		vec3 diffuseIBL = kd * albedo * irradiance;

		// Sample pre-filtered specular reflection environment at correct mipmap level.
		int specularTextureLevels = textureQueryLevels(uPrefilterTex);


		vec3 specularIrradiance = textureLod(uPrefilterTex, Lr, roughness * specularTextureLevels).rgb;

		// Split-sum approximation factors for Cook-Torrance specular BRDF.
		vec2 specularBRDF = texture(uBrdfLUTtex, vec2(cosLo, 1 - roughness)).rg;

		// Total specular IBL contribution.
		vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
		ambientLighting = diffuseIBL + specularIBL;
	}

	// Final fragment color.

	vec3 color = directLighting + ambientLighting;

	//color = pow(color, vec3(1.0/2.2) );

	FragColor = vec4(color, 1.0);

}
