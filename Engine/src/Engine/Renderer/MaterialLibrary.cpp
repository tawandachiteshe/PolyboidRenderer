#include "boidpch.h"
#include "MaterialLibrary.h"


namespace Polyboid
{
	Unique<MaterialDataLibrary> MaterialLibrary::s_Data = std::make_unique<MaterialDataLibrary>();

	Ref<Material>& MaterialLibrary::CreateMaterial(const UUID& id, const std::string& name)
	{
		s_Data->m_Materials[id] = std::make_shared<Material>(name);
		s_Data->m_Materials[id]->SetID(id);

		return s_Data->m_Materials[id];
	}

	Ref<Material> MaterialLibrary::GetMaterial(const UUID& id)
	{

		if (s_Data->m_Materials.find(id) != s_Data->m_Materials.end())
		{
			return s_Data->m_Materials[id];
		}

		return nullptr;

	}
}
