#pragma once
#include "Engine/Engine/UUID.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"


namespace Polyboid
{
	struct MaterialData
	{
		MaterialData() = default;
		glm::vec3 Albedo{ 1.0f };
		glm::vec3 AO{ 1.0f };
		float Roughness = 1.0f;
		float Metallic = 0.5f;
	};

	class Material
	{
	public:

		Material() = default;
		Material(const std::string& name);
		void SetAlbedoColor(const glm::vec4& color);
		glm::vec4& GetAlbedoColor();

		void SetAlbedo(const glm::vec3& color);
		glm::vec3& GetAlbedo();


		void SetAO(const glm::vec3& color);
		glm::vec3& GetAO();


		void SetRoughness(const float& roughness);
		float& GetRoughness();

		void SetMetallic(const float& metallic);
		float& GetMetallic();

		void SetID(const UUID& id);

	private:

		std::string m_Name;
		UUID m_ID;
		MaterialData m_Data;
	};
	
}
