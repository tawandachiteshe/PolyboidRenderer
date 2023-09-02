#pragma once
#include "Engine/Engine/UUID.h"
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include <string>

#include "Engine/Engine/Base.h"

namespace Polyboid
{
	class PipelineDescriptorSet;

	struct MaterialData
	{
		glm::vec3 color;

	};

	class Material
	{
	public:

		virtual std::string& GetName() { return m_Name; }

		virtual MaterialData& GetData() { return m_Data; }

		virtual void SetColor(const glm::vec3& color) = 0;

		virtual ~Material() = default;

		virtual void UploadMaterial() = 0;

		virtual std::vector<Ref<PipelineDescriptorSet>> GetDescSets() = 0;

	protected:
		std::string m_Name;
		UUID m_ID;
		MaterialData m_Data{};
	};
}
