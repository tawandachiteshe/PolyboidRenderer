#version 450 core

out vec4 FragColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;

struct CameraData 
{
	vec3 ViewPosition;
};


uniform CameraData uCameraData;


const float PI = 3.14159265359;

 vec3 F_Schlick(vec3 f0 , float f90 , float u )
 {
	return f0 + (f90 - f0) * pow(1.f - u , 5f);
 }


float Fr_DisneyDiffuse ( float NdotV , float NdotL , float LdotH ,
float linearRoughness )
{
	float energyBias = mix(0.0 , 0.5 , linearRoughness );
	float energyFactor = mix(1.0 , 1.0 / 1.51 , linearRoughness );
	float fd90 = energyBias + 2.0 * LdotH * LdotH * linearRoughness ;
	vec3 f0 = vec3 (1.0f, 1.0f , 1.0f);
	float lightScatter = F_Schlick ( f0 , fd90 , NdotL).r;
	float viewScatter = F_Schlick (f0 , fd90 , NdotV ).r;

	return lightScatter * viewScatter * energyFactor ;
 }

float V_SmithGGXCorrelated ( float NdotL , float NdotV , float alphaG )
{
	// Original formulation of G_SmithGGX Correlated
	// lambda_v = ( -1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5 f;
	 // lambda_l = ( -1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5 f;
	 // G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
	 // V_SmithGGXCorrelated = G_SmithGGXCorrelated / (4.0 f * NdotL * NdotV );

	 // This is the optimize version
	 float alphaG2 = alphaG * alphaG ;
	 // Caution : the " NdotL *" and " NdotV *" are explicitely inversed , this is not a mistake .
	 float Lambda_GGXV = NdotL * sqrt (( - NdotV * alphaG2 + NdotV ) * NdotV + alphaG2 );
	 float Lambda_GGXL = NdotV * sqrt (( - NdotL * alphaG2 + NdotL ) * NdotL + alphaG2 );

	 return 0.5f / ( Lambda_GGXV + Lambda_GGXL );
 }


 float D_GGX ( float NdotH , float m )
 {
	 // Divide by PI is apply later
	 float m2 = m * m ;
	 float f = ( NdotH * m2 - NdotH ) * NdotH + 1;
	 return m2 / (f * f) ;
 }


float smoothDistanceAtt ( float squaredDistance , float invSqrAttRadius )
{
	float factor = squaredDistance * invSqrAttRadius;
	float smoothFactor = clamp(1.0f - factor * factor, 0.0, 1.0);
	return smoothFactor * smoothFactor ;
}

float getDistanceAtt (vec3 unormalizedLightVector , float invSqrAttRadius )
{
	 float sqrDist = dot ( unormalizedLightVector , unormalizedLightVector );
	 float attenuation = 1.0 / (max ( sqrDist , 0.01*0.01) );
	 attenuation *= smoothDistanceAtt ( sqrDist , invSqrAttRadius );

	 return attenuation ;
 }

 float getAngleAtt ( vec3 normalizedLightVector , vec3 lightDir ,
 float lightAngleScale , float lightAngleOffset )
 {
	 // On the CPU
	 // float lightAngleScale = 1.0 f / max (0.001f, ( cosInner - cosOuter ));
	 // float lightAngleOffset = -cosOuter * angleScale ;

	 float cd = dot ( lightDir , normalizedLightVector );
	 float attenuation = clamp( cd * lightAngleScale + lightAngleOffset, 0.0, 1.0 ) ;
	 // smooth the transition
	 attenuation *= attenuation;

	 return attenuation ;
 }


void main()
{

	//Camera view and Normals
	vec3 N = normalize(vNormal);
	vec3 V = normalize(uCameraData.ViewPosition - vPixelPosition);
	vec3 L = vec3(0.23, 1.0, 0.23);

	float roughness = 0.1f;

	 // This code is an example of call of previous functions
	float NdotV = abs(dot(N, V)) + 0.00001; // avoid artifact
	vec3 H = normalize (V + L);
	float LdotH = clamp( dot (L , H ), 0.0, 1.0);
	float NdotH = clamp( dot (N , H ), 0.0, 1.0);
	float NdotL = clamp( dot (N , L ), 0.0, 1.0);

	//Stuff idk
	float f90 = NdotV;
	vec3 albedo = vec3(0.9, 0.6, 0.3);
	vec3 metallic = vec3(0.5);
	vec3 f0 = mix(vec3(0.04), albedo, metallic);
	//vec3 f0 = vec3(1.0);
	
	// Specular BRDF
	vec3 F = F_Schlick(f0, f90 , LdotH);
	float Vis = V_SmithGGXCorrelated ( NdotV , NdotL , roughness );
	float D = D_GGX ( NdotH , roughness );
	vec3 Fr = D * F * Vis / PI;
	
	// Diffuse BRDF
	float Fd = Fr_DisneyDiffuse ( NdotV , NdotL , LdotH , roughness ) / PI;

	FragColor = vec4(Fd.xxx, 1.0f);


}