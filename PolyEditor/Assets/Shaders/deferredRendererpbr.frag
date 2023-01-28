#version 450 core

layout (location = 0) out vec4 FragColor;

layout (binding = 0) uniform sampler2D uAlbedoTex;
layout (binding = 1) uniform sampler2D uNormalTex;
layout (binding = 2) uniform sampler2D uMetalRoughness;
layout (binding = 3) uniform sampler2D uPositionsTex;


in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;
in mat3 vTBN;

const float PI = 3.14159265359;
const vec3 Fdielectric = vec3(0.04);

#define DIRECTIONAL_LIGHT	0
#define SPOT_LIGHT			1
#define POINT_LIGHT			2
#define MAX_LIGHTS			4

struct CameraData 
{
	vec3 ViewPosition;
};



struct DirectionalLight
{
	vec3 Direction;
	vec3 Color;
	float Energy;
};

struct PointLight
{
	vec3 Position;
	vec3 Color;
	float Distance;
	float Energy;
};


struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	float InnerAngle;
	float OuterAngle;
	float Distance;
	float Energy;
	vec3 Color;
};


uniform DirectionalLight uDirectionalLight[MAX_LIGHTS];
uniform PointLight uPointLight[MAX_LIGHTS];
uniform SpotLight uSpotLight[MAX_LIGHTS];
uniform int uPointLightsCount;
uniform int uSpotLightsCount;

uniform float uAmbientStrength = 0.1;
layout (binding = 5) uniform samplerCube uPrefilterTex;
layout (binding = 6) uniform sampler2D uBrdfLUTtex;
layout (binding = 7) uniform samplerCube uIrradianceTex;
layout (binding = 8) uniform samplerCube uEnvTex;

uniform CameraData uCameraData;
uniform int uLightType[MAX_LIGHTS];

vec3 CalculateL(int lightIndex);

float CalculateAttenuation(PointLight light)
{

	float Linear = 0.0;
	float Quadratic = 0.5;
	float Distance = light.Distance;
	float Energy = light.Energy;
	
	float r = length(light.Position - vPixelPosition);
	float I = Energy * (Distance / (Distance + Linear * r)) * (Distance * Distance / ((Distance * Distance) + Quadratic * (r * r)));
	float Sphere = I * (Distance - r) / Distance * float(r < Distance);
	return Sphere;
}

float CalculateAttenuationV2(float Range, float Distance)
{
	return 1.0 - smoothstep(Range * 0.75, Range, Distance);
}


float CalculateSpotCone( SpotLight light, int lightIndex )
{
	// If the cosine angle of the light's direction
	// vector and the vector from the light source to the point being
	// shaded is less than minCos, then the spotlight contribution will be 0.
	float minCos = cos( radians( light.InnerAngle ) );
	// If the cosine angle of the light's direction vector
	// and the vector from the light source to the point being shaded
	// is greater than maxCos, then the spotlight contribution will be 1.
	float maxCos = mix( minCos, 1.0, 0.5f );

	float cosAngle = dot( light.Direction, -CalculateL(lightIndex) );
	// Blend between the minimum and maximum cosine angles.
	return smoothstep( minCos, maxCos, cosAngle );
}


vec3 UnpackNormals(vec3 normals)
{
	return normals * 2.0f - 1.0f;
}


float CalculateLightDistance(int lightIndex)
{
	switch(uLightType[lightIndex])
	{
		case SPOT_LIGHT:
			return length(uSpotLight[lightIndex].Position - vPixelPosition);
		case POINT_LIGHT:
			return length(uPointLight[lightIndex].Position - vPixelPosition);
	}
}


vec3 DirectionalLightL(int lightIndex)
{
	vec3 L = normalize(uDirectionalLight[lightIndex].Direction);
	return L;
}

vec3 PointLightL(int lightIndex)
{
	vec3 L = uPointLight[lightIndex].Position - vPixelPosition;
	float Distance = CalculateLightDistance(lightIndex);
	L = L / Distance;
	return L;
}


vec3 SpotLightL(int lightIndex)
{
	
	vec3 L = uSpotLight[lightIndex].Position - vPixelPosition;
	float Distance = CalculateLightDistance(lightIndex);
	L = L / Distance;
	return L;
}

vec3 CalculateL(int lightIndex)
{
	switch(uLightType[lightIndex])
	{
		case DIRECTIONAL_LIGHT:
			return DirectionalLightL(lightIndex);
		case SPOT_LIGHT:
			return SpotLightL(lightIndex);
		case POINT_LIGHT:
			return PointLightL(lightIndex);
	}
}

vec3 CalculateColor(int lightIndex)
{
	switch(uLightType[lightIndex])
	{
		case DIRECTIONAL_LIGHT:
			return uDirectionalLight[lightIndex].Color;
		case SPOT_LIGHT:
			return uSpotLight[lightIndex].Color;
		case POINT_LIGHT:
			return uPointLight[lightIndex].Color;
	}
}


vec3 CalculateRadiance(int lightIndex)
{	
	switch(uLightType[lightIndex])
	{
		case DIRECTIONAL_LIGHT:
			return  CalculateColor(lightIndex) * uDirectionalLight[lightIndex].Energy;
		case SPOT_LIGHT:
			float spotIntensity = CalculateSpotCone(uSpotLight[lightIndex], lightIndex);
			return CalculateColor(lightIndex) * CalculateAttenuationV2(uSpotLight[lightIndex].Distance, CalculateLightDistance(lightIndex)) * uSpotLight[lightIndex].Energy * spotIntensity;
		case POINT_LIGHT:
			return CalculateColor(lightIndex) * CalculateAttenuationV2(uPointLight[lightIndex].Distance, CalculateLightDistance(lightIndex)) * uPointLight[lightIndex].Energy;
	}
	
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





struct ScreenView
{
	mat4 InverseProjection;
	vec2 ScreenDimensions;
};



void main()
{
	vec2 uv = vUV;

	// Everything is in view space.
	vec4 eyePos = { 0, 0, 0, 1 };

	//uv.x = -uv.x;

	vec3 positions = texture(uPositionsTex, uv).xyz;
	vec3 albedo = texture(uAlbedoTex, uv).xyz;
	vec3 normals = texture(uNormalTex, uv).xyz;
	vec3 metallic = texture(uMetalRoughness, uv).xyz;

	vec3 N = normals;
	float metalness = metallic.b;
	float roughness = metallic.g;



	vec3 Lo = normalize(uCameraData.ViewPosition - positions);

		// Angle between surface normal and outgoing light direction.
	float cosLo = max(0.0, dot(N, Lo));
		
	// Specular reflection vector.
	vec3 Lr = 2.0 * cosLo * N - Lo;

	// Fresnel reflectance at normal incidence (for metals use albedo color).
	vec3 F0 = mix(Fdielectric, albedo, metalness);

	// Direct lighting calculation for analytical lights.
	vec3 directLighting = vec3(0);
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		vec3 Li = -CalculateL(i);
		vec3 Lradiance = CalculateRadiance(i);

		// Half-vector between Li and Lo.
		vec3 Lh = normalize(Li + Lo);

		// Calculate angles between surface normal and various light vectors.
		float cosLi = max(0.0, dot(N, Li));
		float cosLh = max(0.0, dot(N, Lh));

		// Calculate Fresnel term for direct lighting. 
		vec3 F  = fresnelSchlick(F0, max(0.0, dot(Lh, Lo)));
		// Calculate normal distribution for specular BRDF.
		float D = ndfGGX(cosLh, roughness);
		// Calculate geometric attenuation for specular BRDF.
		float G = gaSchlickGGX(cosLi, cosLo, roughness);

		// Diffuse scattering happens due to light being refracted multiple times by a dielectric medium.
		// Metals on the other hand either reflect or absorb energy, so diffuse contribution is always zero.
		// To be energy conserving we must scale diffuse BRDF contribution based on Fresnel factor & metalness.
		vec3 kd = mix(vec3(1.0) - F, vec3(0.0), metalness);

		// Lambert diffuse BRDF.
		// We don't scale by 1/PI for lighting & material units to be more convenient.
		// See: https://seblagarde.wordpress.com/2012/01/08/pi-or-not-to-pi-in-game-lighting-equation/
		vec3 diffuseBRDF = kd * albedo;

		// Cook-Torrance specular microfacet BRDF.
		vec3 specularBRDF = (F * D * G) / max(0.00001, 4.0 * cosLi * cosLo);

		// Total contribution for this light.
		directLighting += (diffuseBRDF + specularBRDF) * Lradiance * cosLi;
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


	FragColor = vec4(color ,1.0f);
}