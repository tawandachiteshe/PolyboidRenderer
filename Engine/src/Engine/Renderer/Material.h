#pragma once
#include "Engine/Engine/UUID.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include <string>

namespace Polyboid
{
	struct MaterialData
	{
		MaterialData() = default;
		glm::vec3 Albedo{1.0f};
		glm::vec3 AO{1.0f};
		
		float Metallic = 0.5f;
		float Roughness = 1.0f;

		int32_t textures[4] = {-1, -1, -1, -1};

		MaterialData(const MaterialData& data): Albedo(data.Albedo), AO(data.AO), Metallic(data.Metallic), Roughness(data.Roughness) {}

	};

	class Material
	{
	public:
		UUID mDiffuseTexture = 0;
		UUID mRoughnessTexture = 0;
		UUID mMetallicTexture = 0;
		UUID mAOTexture = 0;
		UUID mNormalsTexture = 0;

		std::string& GetName() { return m_Name; }

		Material() = default;
		Material(const std::string& name);

		void SetData(const MaterialData& data) { m_Data = data; }

		void SetTextures(int32_t* textures)
		{
			m_Data.textures[0] = textures[0];
			m_Data.textures[1] = textures[1];
			m_Data.textures[2] = textures[2];
			m_Data.textures[3] = textures[3];
		}

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

		MaterialData& GetData() { return m_Data; }

	private:
		std::string m_Name;
		UUID m_ID;
		MaterialData m_Data;
	};
}
