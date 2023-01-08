#pragma once
#include "Material.h"
#include "Engine/Engine/Base.h"
#include <unordered_map>

namespace Polyboid
{

	struct MaterialDataLibrary
	{
		std::unordered_map<UUID, Ref<Material>> m_Materials;
	};

	class MaterialLibrary
	{
	private:
		static Unique<MaterialDataLibrary> s_Data;
	public:
		static Ref<Material>& CreateMaterial(const UUID& id, const std::string& name);
		static Ref<Material> GetMaterial(const UUID& id);
	};
}
