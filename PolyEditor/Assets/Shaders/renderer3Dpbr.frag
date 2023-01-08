#version 450 core

out vec4 FragColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;

const float PI = 3.14159265359;

#define DIRECTIONAL_LIGHT	0
#define SPOT_LIGHT			1
#define POINT_LIGHT			2
#define MAX_LIGHTS			8


struct MaterialData 
{
	vec3 Albedo;
	vec3 AO;
	float Metallic;
	float Roughness;
};

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


uniform MaterialData uMaterial;
uniform DirectionalLight uDirectionalLight[MAX_LIGHTS];
uniform PointLight uPointLight[MAX_LIGHTS];
uniform SpotLight uSpotLight[MAX_LIGHTS];
uniform int uPointLightsCount;
uniform int uSpotLightsCount;

uniform float uAmbientStrength = 0.1;
layout (binding = 0) uniform samplerCube prefilterMap;
layout (binding = 1) uniform sampler2D brdfLUT;
layout (binding = 2) uniform samplerCube irradianceMap;
layout (binding = 5) uniform samplerCube envMap;
layout (binding = 3) uniform sampler2D normalMap;
layout (binding = 4) uniform sampler2D uShadowMap;

uniform CameraData uCameraData;
uniform int uLightType[MAX_LIGHTS];

vec3 CalculateL(int lightIndex);

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;
	float num = NdotV;
	float denom = NdotV * (1.0 - k) + k;
	return num / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	return ggx1 * ggx2;
}



float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 = a*a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;
	float num = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	return num / denom;
}



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


vec3 FresnelSchlick(float cosTheta, vec3 F0, float exponent)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, exponent);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) *
	pow(1.0 - cosTheta, 5.0);
}

vec3 GetNormalVector(vec2 uv)
{
	vec2 m = texture(normalMap, uv).xy;
	return vec3(m, sqrt(1.0 - m.x * m.x - m.y * m.y));
}

vec3 UnpackNormals(vec3 normals)
{
	return normals * 2.0f - 1.0f;
}


vec3 CalculateCookTorrance(vec3 N, vec3 H, vec3 L, vec3 V, vec3 F0)
{
	vec3 albedo = uMaterial.Albedo;
	vec3 AO = uMaterial.AO;
	float metallic = uMaterial.Metallic;
	float roughness = uMaterial.Roughness;

		// cook-torrance brdf
	float NDF = DistributionGGX(N, H, roughness);
	float G = GeometrySmith(N, V, L, roughness);
	
	//No textureMapping

	/*
	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
	vec3 _specular = numerator / max(denominator, 0.001);
	*/


	return G.xxx;
}


vec3 CalculateDiffuse(vec3 N) 
{
	vec3 albedo = uMaterial.Albedo;
	
	//diffuse map
	vec3 irradiance = texture(irradianceMap, N).xyz;
	vec3 diffuse = irradiance * albedo;

	return diffuse;

}


vec3 CalculateSpecular(vec3 V, vec3 N, vec3 F)
{
	
	float roughness = uMaterial.Roughness;
	
	vec3 R = reflect(-V, N);
	const float MAX_REFLECTION_LOD = 1.5;

	float roughness01 = (roughness) * MAX_REFLECTION_LOD;

	vec3 prefilteredColor = textureLod(prefilterMap, R, roughness01).rgb;

	vec2 uv = vec2(max(dot(N, V), 0.0), roughness);
	uv.x = -uv.x;

	vec2 envBRDF = texture(brdfLUT, uv).rg;
	vec3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);

	return specular;
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


vec3 DirectionalLightL()
{
	vec3 L = normalize(uDirectionalLight[0].Direction);
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
			return DirectionalLightL();
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

void main()
{
	vec3 albedo = uMaterial.Albedo;
	vec3 AO = uMaterial.AO;
	float metallic = uMaterial.Metallic;
	float roughness = uMaterial.Roughness;

	//Camera view and Normals
	vec3 N = normalize(vNormal);
	vec3 V = normalize(uCameraData.ViewPosition - vPixelPosition);
	
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	
	// reflectance equation
	vec3 Lo = vec3(0.0);

	
	//Cook torrance
	vec3 F = fresnelSchlickRoughness(clamp(dot(N, V), 0.0, 1.0), F0, roughness);

	//specular from map
	vec3 specular = CalculateSpecular(V, N, F);
	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallic;

	//diffuse map
	vec3 diffuse = CalculateDiffuse(N);
	
	for(int i = 0; i < MAX_LIGHTS; i++)
	{
		// calculate per-light radiance
		vec3 lightColor = CalculateColor(i);
		vec3 L = CalculateL(i);
		vec3 radiance = CalculateRadiance(i);

		// add to outgoing radiance Lo
		float NdotL = max(dot(N, L), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}



	//Ambient no map
	vec3 _ambient = vec3(0.03) * albedo * AO;

	//ambient widthMap
	vec3 ambient = (kD * diffuse + specular) * AO;

	vec3 color = ambient + Lo;
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0/2.2));


	FragColor = vec4(color, 1.0f);
}
