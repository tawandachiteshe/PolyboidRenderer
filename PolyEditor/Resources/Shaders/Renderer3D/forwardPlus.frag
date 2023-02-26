#version 450 core


#include "Common/Common.frag.glsl"
#include "Common/CommonFunctions.glsl"
#include "Common/CommonLighting.frag.glsl"



layout(location = 0) out vec4 FragColor;

layout(location = 0) in VS_OUT
{
	vec3 vTangentVS;
	vec3 vBinormalVS;
	vec3 vNormalVS;
	vec2 vUV;
	vec3 vPositionWS;
	vec4 vPositionVS;
	mat4 model;

} vs_in;



layout(binding = 0) uniform sampler2D uTextures[5];
layout(binding = 6) uniform usampler2D uPointLightGrid;
layout(binding = 7) uniform usampler2D uSpotLightGrid;
layout(binding = 8) uniform usampler2D uDirLightGrid;

layout (binding = 28) uniform samplerCube uPrefilterTex;
layout (binding = 29) uniform sampler2D uBrdfLUTtex;
layout (binding = 30) uniform samplerCube uIrradianceTex;
layout (binding = 31) uniform samplerCube uEnvTex;



vec3 getAlbedo()
{
	return vec3(in_Materials[0].Albedo[0], in_Materials[0].Albedo[1], in_Materials[0].Albedo[2]);
}

int getTextureIndex(int textureIdx)
{

	return in_Materials[0].textures[textureIdx];
}

vec4 getTexture(int textureIdx)
{
	switch(getTextureIndex(textureIdx))
	{
		case 0:
			return texture(uTextures[0], vs_in.vUV);
		case 1:
			return texture(uTextures[1], vs_in.vUV);
		case 2:
			return texture(uTextures[2], vs_in.vUV);
		case 3:
			return texture(uTextures[3], vs_in.vUV);
		case 4:
			return texture(uTextures[4], vs_in.vUV);
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
		return normalize(vs_in.vNormalVS);
	} else {

		mat3 TBN = mat3( normalize( vs_in.vTangentVS ),
                         normalize( vs_in.vBinormalVS ),
                         normalize( vs_in.vNormalVS ) );
	
		vec3 N = normalize(2.0 * getTexture(1).rgb - 1.0);
		return normalize(TBN * N);
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
	SurfaceData data;
	vec3 albedo = getAlbedoTexture();
	data.albedo = albedo;

	vec3 metalnessRoughness = getMetalRougnessTexture();

	float metalness = metalnessRoughness.b;
	float roughness = metalnessRoughness.g;

	if(metalness == 1.0f)
	{
		metalness = in_Materials[0].Metallic;
	}

	if(roughness == 1.0f)
	{
		roughness = in_Materials[0].Roughness;
	}

	data.roughness = roughness;
	data.metalness = metalness;

	vec3 eyePos = { 0, 0, 0 };

	// Outgoing light direction (vector from world-space fragment position to the "eye").
	vec3 Lo = normalize(eyePos - vs_in.vPositionVS.xyz);
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

	//Light Grid

	uvec2 tileIndex = uvec2(floor(gl_FragCoord.xy / 16));
	uvec2 pointlightGrid = texture(uPointLightGrid, tileIndex).xy;
	uvec2 spotlightGrid = texture(uSpotLightGrid, tileIndex).xy;
	uvec2 dirlightGrid = texture(uDirLightGrid, tileIndex).xy;


	uint startPointOffset = pointlightGrid.x;
	uint pointLightCount = pointlightGrid.y;


	uint startSpotOffset = spotlightGrid.x;
	uint spotLightCount = spotlightGrid.y;

	uint startDirOffset = pointlightGrid.x;
	uint dirLightCount = dirlightGrid.y;


	//Point Lights
	for(int i = 0; i < pointLightCount; ++i)
	{

		uint lightIndex = lightListIndex[startPointOffset + i].Point;
	
		vec3 Li = PointLightL(int(lightIndex), vec3(vs_in.vPositionVS));
		vec3 Lradiance = PointLightRadiance(int(lightIndex), vec3(vs_in.vPositionVS));

		directLighting += CalculateDirectLight(Li, Lradiance, data);
	}


		//Point Lights
	for(int i = 0; i < dirLightCount; ++i)
	{
		uint lightIndex = lightListIndex[startDirOffset + i].Dir;

		vec3 Li = DirectionalLightL(int(lightIndex));
		vec3 Lradiance = DirLightRadiance(int(lightIndex));

		directLighting += CalculateDirectLight(Li, Lradiance, data);
	}

			//Point Lights
	for(int i = 0; i < spotLightCount; ++i)
	{

		uint lightIndex = lightListIndex[startSpotOffset + i].Spot;
	
		vec3 Li = SpotLightL(int(lightIndex), vec3(vs_in.vPositionVS));
		vec3 Lradiance = SpotRadiance(int(lightIndex), vec3(vs_in.vPositionVS));

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
		vec2 specularBRDF = texture(uBrdfLUTtex, vec2(cosLo, roughness)).rg;

		// Total specular IBL contribution.
		vec3 specularIBL = (F0 * specularBRDF.x + specularBRDF.y) * specularIrradiance;

		// Total ambient lighting contribution.
		ambientLighting = diffuseIBL + specularIBL;
	}

	// Final fragment color.

	vec3 color = directLighting + ambientLighting;

	//color = pow(color, vec3(1.0/2.2) );

	 FragColor = vec4(color, 1.0);


	//FragColor = vec4(color, 1.0);
						 
}

/*
float LinearDepth(float depthSample){
    float depthRange = 2.0 * depthSample - 1.0;
    // Near... Far... wherever you are...
    float linear = 2.0 * zNear * zFar / (zFar + zNear - depthRange * (zFar - zNear));
    return linear;
}
*/
