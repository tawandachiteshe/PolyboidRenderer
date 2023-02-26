#ifndef COMMON_FRAG
#define COMMON_FRAG

struct DirectionalLight
{
	float Direction[3];
	float DirectionVS[3];
	float Color[3];
	float Energy;
	bool Enabled;
};

struct PointLight
{
	float Position[3];
	float PositionVS[3];
	float Color[3];
	float Distance;
	float Energy;
	bool Enabled;
};


struct SpotLight
{
	float Position[3];
	float PositionVS[3];
	float Direction[3];
	float DirectionVS[3];
	float Color[3];
	float InnerAngle;
	float OuterAngle;
	float Distance;
	float Energy;
	bool Enabled;
};

struct MaterialData
{
	float Albedo[3];
	float AO[3];
	float Metallic;
	float Roughness;
	int textures[4];

};

struct LightCount
{
    uint Point;
    uint Spot;
    uint Dir;
};


layout (std140, binding = 0) uniform CameraBuffer {
    
	uniform mat4 projection;
    uniform mat4 view;
	uniform vec3 cameraPosition;
	uniform mat4 transform;
};

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

layout (std430, binding = 5) buffer LightCountSSBO{
    LightCount lightCount;
};


layout (std430, binding = 6) buffer LightIndexList{
    LightCount lightListIndex[];
};


#endif  //End common frag