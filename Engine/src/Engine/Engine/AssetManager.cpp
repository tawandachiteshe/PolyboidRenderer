#include "boidpch.h"
#include "AssetManager.h"

#include <spdlog/spdlog.h>

#include "Engine/Renderer/VertexBufferArray.h"
#include "MeshImporter/MeshImporter.h"
#include "Engine/Renderer/Material.h"
#include "Engine/Renderer/Renderer.h"
#include "Engine/Renderer/ShaderBufferStorage.h"
#include "Engine/Renderer/Texture2D.h"


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

	RenderData AssetManager::LoadRenderData(const std::string& AssetName)
	{
		auto& meshData = GetMesh(AssetName);


		if(s_Data->m_RendererMeshes.find(AssetName) != s_Data->m_RendererMeshes.end())
		{
			return s_Data->m_RendererMeshes[AssetName];
		}


		RenderData data;

		for (auto& mesh : meshData)
		{
			auto& [mat, meshVertsAndIdx] = mesh;


			auto indicesBuffer = ShaderBufferStorage::Make(meshVertsAndIdx.Indices.data(), meshVertsAndIdx.Indices.size() * sizeof(uint32_t));

			auto va = VertexBufferArray::MakeVertexBufferArray(indicesBuffer, meshVertsAndIdx.Indices.size());

			auto verts = ShaderBufferStorage::Make(meshVertsAndIdx.vertices.data(), meshVertsAndIdx.vertices.size() * sizeof(RendererVertex));
			va->SetShaderBufferStorage(verts);

			uint64_t ui = 0;

			if (mat->mDiffuseTexture != ui)
			{
				LoadTexture(mat->mDiffuseTexture, Texture::MakeTexture2D(GetTextureData(mat->mDiffuseTexture)));
			}

			if (mat->mNormalsTexture != ui)
			{
				LoadTexture(mat->mNormalsTexture, Texture::MakeTexture2D(GetTextureData(mat->mNormalsTexture)));
			}

			if (mat->mMetallicTexture != ui)
			{
				LoadTexture(mat->mMetallicTexture, Texture::MakeTexture2D(GetTextureData(mat->mMetallicTexture)));
			}

			if (mat->mAOTexture != ui)
			{
				LoadTexture(mat->mAOTexture, Texture::MakeTexture2D(GetTextureData(mat->mAOTexture)));
			}

			if (mat->mRoughnessTexture != ui)
			{
				LoadTexture(mat->mRoughnessTexture, Texture::MakeTexture2D(GetTextureData(mat->mRoughnessTexture)));
			}


			data[mat] = va;
		}

		s_Data->m_RendererMeshes[AssetName] = data;

		return data;
	}

	RenderData& AssetManager::GetRenderData(const std::string& AssetName)
	{
		return s_Data->m_RendererMeshes[AssetName];
	}

	void AssetManager::LoadTexture(const UUID& id, const Ref<Texture>& texture)
	{
		s_Data->m_Textures[id] = texture;
	}

	void AssetManager::LoadTextureData(const UUID& id, const Ref<TextureData>& textureData)
	{

		s_Data->m_TexturesData[id] = textureData;
	}

	Ref<Texture>& AssetManager::GetTexture(const UUID& id)
	{
		return s_Data->m_Textures[id];
	}

	Ref<TextureData>& AssetManager::GetTextureData(const UUID& id)
	{

		return s_Data->m_TexturesData[id];

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

