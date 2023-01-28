#include "boidpch.h"
#include "AssetManager.h"

#include <spdlog/spdlog.h>

#include "Engine/Renderer/VertexBufferArray.h"
#include "MeshImporter/MeshImporter.h"
#include "Engine/Renderer/Material.h"


namespace Polyboid
{
	Unique<Data> AssetManager::s_Data = std::make_unique<Data>();

	static std::filesystem::path s_WorkDir = "Assets";
	static std::filesystem::path s_CurrentDir = s_WorkDir;



	MeshDataRenderer& AssetManager::LoadMesh(const std::string& path)
	{

		std::filesystem::path fPath = path;
		auto fileName = fPath.filename().string();

		if (s_Data->m_Meshes.find(fileName) != s_Data->m_Meshes.end())
		{
			return s_Data->m_Meshes[fileName];
		}

		s_Data->m_Meshes[fileName] = MeshImporter::ReadForRendering(path);
		s_Data->m_LoadedMeshNames.push_back(fileName);
		s_Data->m_CStrLoadedMeshNames.push_back(fileName.c_str());

		return  s_Data->m_Meshes[fileName];
	}

	MeshDataRenderer& AssetManager::GetMesh(const std::string& AssetName)
	{
		// if (s_Data->m_Meshes.find(AssetName) != s_Data->m_Meshes.end())
		// {

		return s_Data->m_Meshes[AssetName];
	}

	void AssetManager::LoadTexture(const UUID& id, const Ref<Texture>& texture)
	{
		s_Data->m_Textures[id] = texture;
	}

	Ref<Texture>& AssetManager::GetTexture(const UUID& id)
	{
		return s_Data->m_Textures[id];
	}

	void AssetManager::Init()
	{
		for (auto& object : std::filesystem::recursive_directory_iterator(s_CurrentDir))
		{


			if (!std::filesystem::is_directory(object))
			{
				if (object.path().extension() == ".polymesh")
				{
					auto path = object.path().string();
					spdlog::info("{}", path);
					LoadMesh(path);
				}
				
			}
		}
	}

	std::vector<std::string>& AssetManager::LoadedMeshes()
	{
		return s_Data->m_LoadedMeshNames;
	}

	std::vector<const char*>& AssetManager::CStrLoadedMeshes()
	{
		return s_Data->m_CStrLoadedMeshNames;
	}
}

