#include "Common/Common.frag.glsl"
#include "Common/CommonFunctions.glsl"

#ifndef COMMON_LIGHTING
#define COMMON_LIGHTING

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


float CalculateAttenuationV2(float Range, float Distance)
{
	return pow(clamp(1.0 - pow((Distance / Range), 4.0), 0.0, 1.0), 2.0)/(1.0  + (Distance * Distance) );
}


float PointLightDistance(int lightIndex, vec3 vPositionVS)
{
	return length(float3ToVec3(in_PointLights[lightIndex].PositionVS) - vPositionVS);
}

vec3 PointLightL(int lightIndex, vec3 vPositionVS)
{
	vec3 L = normalize(float3ToVec3(in_PointLights[lightIndex].PositionVS) - vPositionVS);
	return L;
}

vec3 PointLightColor(int lightIndex)
{
	return float3ToVec3(in_PointLights[lightIndex].Color);
}

vec3 PointLightRadiance(int lightIndex, vec3 vPositionVS)
{
	float attenuation = CalculateAttenuationV2(in_PointLights[lightIndex].Distance, PointLightDistance(lightIndex, vPositionVS));
	
	return PointLightColor(lightIndex) * attenuation * in_PointLights[lightIndex].Energy;
}


vec3 DirectionalLightL(int lightIndex)
{
	vec3 L = normalize(-float3ToVec3(in_DirLights[lightIndex].DirectionVS));
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


float SpotLightDistance(int lightIndex, vec3 vPositionVS)
{
	return length(float3ToVec3(in_SpotLights[lightIndex].PositionVS) - vPositionVS.xyz);
}

vec3 SpotLightL(int lightIndex,  vec3 vPositionVS)
{
	
	vec3 L = float3ToVec3(in_SpotLights[lightIndex].PositionVS) - vPositionVS.xyz;
	float Distance = SpotLightDistance(lightIndex, vPositionVS);
	L = L / Distance;
	return L;
}

float CalculateSpotCone(int lightIndex,  vec3 vPositionVS)
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

	float cosAngle = dot( float3ToVec3(light.DirectionVS), -SpotLightL(lightIndex, vPositionVS) );
	// Blend between the minimum and maximum cosine angles.
	return smoothstep( minCos, maxCos, cosAngle );
}

vec3 SpotLightColor(int lightIndex)
{
	return float3ToVec3(in_SpotLights[lightIndex].Color); 
}

vec3 SpotRadiance(int lightIndex,  vec3 vPositionVS)
{
	float spotIntensity = CalculateSpotCone(lightIndex, vPositionVS);

	float spotDistance = SpotLightDistance(lightIndex, vPositionVS);
	float lightIntensity = in_SpotLights[lightIndex].Energy * spotIntensity;
	float attenuation = CalculateAttenuationV2(in_SpotLights[lightIndex].Distance, spotDistance) * lightIntensity;

	return SpotLightColor(lightIndex) * attenuation;
}


vec3 CalculateDirectLight(vec3 Li, vec3 Lradiance, SurfaceData data)
{

	vec4 eyePos = vec4(0.0, 0.0, 0.0, 1.0);
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

vec4 ClipToView( vec4 clip )
{
    // View space position.
    vec4 view = inverse(projection) * clip;
    // Perspective projection.
    view = view / view.w;
    return view;
}

#endif //common lighting