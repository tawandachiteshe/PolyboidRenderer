#version 450 core

out vec4 FragColor;
in vec2 vUV;
in vec3 vNormal;
in vec3 vPixelPosition;
in vec3 vPosition;

struct MaterialData 
{
	vec4 AlbedoColor;
	vec3 Ambient;
	vec3 Diffuse;
	vec3 Specular;
	float Roughness;
	float _Roughness;

};

struct CameraData 
{
	vec3 ViewPosition;
};



struct DirectionalLight
{
	vec3 Position;
	vec3 Direction;
	vec3 Color;
};

struct PointLight
{
	vec3 Position;
	vec3 Color;
	float Linear;
	float Quadratic;
	float Distance;
	float Energy;
};


struct SpotLight
{
	vec3 Position;
	vec3 Direction;
	float InnerAngle;
	float OuterAngle;
	vec3 Color;
};


uniform MaterialData uMaterial;
uniform DirectionalLight uDirectionalLight;
uniform PointLight uPointLight;
uniform SpotLight uSpotLight;
uniform float uAmbientStrength = 0.1;


uniform CameraData uCameraData;
uniform int uLightType;


float InverseSquareAttenuation(vec3 lightPosition, vec3 Attentuation)
{
	float r2 = length(lightPosition - vPixelPosition);
	return clamp(Attentuation.x * (Attentuation.y / r2 - 1.0), 0.0, 1.0);
}

float ExponentialAttenuation(vec3 lightPosition, vec3 Attentuation)
{
	float r2 = length(lightPosition - vPixelPosition);
	return clamp(exp(r2 * Attentuation.x) * Attentuation.y - Attentuation.z, 0.0, 1.0);
}

float SmoothAttenuation(vec3 lightPosition, vec3 Attentuation)
{
	float r2 = length(lightPosition - vPixelPosition);
	return clamp(r2 * Attentuation.x * (sqrt(r2) * Attentuation.y - 3.0) + 1.0, 0.0, 1.0);
}


float CalculateLambert(vec3 L, vec3 N)
{
	float lambert = clamp(dot(N, L), 0.0, 1.0);
	return lambert;
}

vec3 CalculateSpecular(vec3 L, vec3 N, vec3 lightColor) 
{
	float lambert = CalculateLambert(N, L);
	//Specular
	vec3 V = normalize(uCameraData.ViewPosition - vPixelPosition);
	vec3 H = normalize(L + V);
	//vec3 R = reflect( -L, N ); uses phong
	
	//blinn - phong
	float specularLight = clamp(dot(H, N), 0.0, 1.0) * float(lambert > 0.0);
	
	//float specularLight = clamp(dot(V, R), 0.0, 1.0);

	specularLight = pow(specularLight, uMaterial.Roughness) * uMaterial._Roughness;

	return specularLight * uMaterial.Specular * lightColor;
}

vec3 CalculateAmbient() 
{
	return uAmbientStrength * uMaterial.Diffuse;
}


vec3 CalculateDiffuse(vec3 L, vec3 N, vec3 lightColor) 
{
	float lambert = CalculateLambert(N, L);
	return lambert * uMaterial.Diffuse * lightColor;
}


float CalculateFresnel(vec3 N, float fresnelExp)
{
	vec3 V = normalize(uCameraData.ViewPosition - vPixelPosition);
	float fresnel = pow((1.0 - clamp(dot(V, N), 0.0, 1.0)), fresnelExp);
	return fresnel;
}

vec3 CalculateBasicLighting(vec3 L, vec3 N, vec3 lightColor) 
{


	//Ambient
	float ambientStrength = 0.5;

	//All together 
	vec3 diffuse = CalculateDiffuse(L, N, lightColor);
	vec3 specular = CalculateSpecular(L, N, lightColor);
	vec3 ambient = CalculateAmbient();
	

	//float fresnel = pow((1.0 - clamp(dot(V, N), 0.0, 1.0)), 4);

	vec3 result = ambient + diffuse + specular;

	return result; //(ambient + diffuse + specular);
}

vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{

	//Diffuse lighting
	vec3 N = normal;
	vec3 L = normalize(light.Direction);

	return CalculateBasicLighting(L, N, light.Color);

}

float ComputeAttentuation(vec3 lightPosition, vec3 Attentuation, int AttentuationType) 
{
	switch(AttentuationType) 
	{
	case 0:
		return InverseSquareAttenuation(lightPosition, Attentuation);
	case 1:
		return ExponentialAttenuation(lightPosition, Attentuation);
	case 2:
		return SmoothAttenuation(lightPosition, Attentuation);
	}

}


float CalculateAttenuation(PointLight light)
{

	float Linear = light.Linear;
	float Quadratic = light.Quadratic;
	float Distance = light.Distance;
	float Energy = light.Energy;
	
	float r = length(light.Position - vPixelPosition);
	float I = Energy * (Distance / (Distance + Linear * r)) * (Distance * Distance / ((Distance * Distance) + Quadratic * (r * r)));
	float Sphere = I * (Distance - r) / Distance * float(r < Distance);

	return Sphere;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDirection) 
{
	
	
	vec3 N = normal;

	float r = length(light.Position - vPixelPosition);

	vec3 L = light.Position - vPixelPosition;

	float attenuation = CalculateAttenuation(light);
	//attenuation = ComputeAttentuation(light.Position, light.Attenuation, light.AttenuationType);

	vec3 lightColor = light.Color;

	L /= r;
	vec3 diffuse = CalculateDiffuse(L, N, lightColor);
	vec3 specular = CalculateSpecular(L, N, lightColor);
	vec3 ambient = CalculateAmbient();
	 

	//ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	
	//float fresnel = CalculateFresnel(N, light.Distance);

	vec3 result = ambient + diffuse + specular;

	return result;
}



vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDirection) 
{
	return vec3(1.0f);
}


void main() {   

	vec3 color = vec3(0);
	vec3 normal = normalize(vNormal);
	vec3 viewDir = normalize(uCameraData.ViewPosition - vPixelPosition);

	switch(uLightType)
	{
		case 0:
			color = CalculateDirectionalLight(uDirectionalLight, normal, viewDir);
			break;
		case 1:
			color = CalculatePointLight(uPointLight, normal, viewDir);
			break;
		case 2:
			color = CalculateSpotLight(uSpotLight, normal, viewDir);
			break;
	}


	FragColor = vec4(color, 1.0f);

}