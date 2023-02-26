#pragma once
#include "glm/vec3.hpp"


struct DirectionalLightData
{
	DirectionalLightData() = default;
	glm::vec3 Direction{};
	glm::vec3 DirectionVS{};
	glm::vec3 Color{};
	float Energy{};
	bool Enabled = false;
};

struct PointLightData
{
	PointLightData() = default;
	glm::vec3 Position{};
	glm::vec3 PositionVS{};
	glm::vec3 Color{};
	float Distance{};
	float Energy{};
	bool Enabled = false;
};

struct SpotLightData
{
	SpotLightData() = default;

	glm::vec3 Position{};
	glm::vec3 PositionVS{};
	glm::vec3 Direction{};
	glm::vec3 DirectionVS{};
	glm::vec3 Color{};

	float InnerAngle = 0.0f;
	float OuterAngle = 0.0f;
	float Distance = 0.0f;
	float Energy = 0.0f;

	bool Enabled = false;
};