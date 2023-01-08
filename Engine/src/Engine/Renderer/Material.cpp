#include "boidpch.h"
#include "Material.h"



namespace Polyboid
{
	Material::Material(const std::string& name): m_Name(name), m_ID(UUID())
	{
		
	}

	void Material::SetAlbedo(const glm::vec3& color)
	{
		m_Data.Albedo = color;
	}

	glm::vec3& Material::GetAlbedo()
	{
		return  m_Data.Albedo;
	}

	void Material::SetAO(const glm::vec3& color)
	{
		m_Data.AO = color;
	}

	glm::vec3& Material::GetAO()
	{
		return m_Data.Albedo;
	}

	void Material::SetRoughness(const float& roughness)
	{
		m_Data.Roughness = roughness;
	}

	float& Material::GetRoughness()
	{
		return m_Data.Roughness;
	}

	void Material::SetMetallic(const float& metallic)
	{
		m_Data.Metallic = metallic;
	}

	float& Material::GetMetallic()
	{
		return m_Data.Metallic;
	}



	void Material::SetID(const UUID& id)
	{
		m_ID = id;
	}
}
