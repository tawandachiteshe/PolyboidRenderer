#pragma once
#include "Base.h"
#include "TextureData.h"
#include "UUID.h"
#include "MeshImporter/MeshImporter.h"


namespace Polyboid
{
	class Material;
	class Texture;
	class VertexBufferArray;

	struct MaterialData;

	struct Data
	{
		std::unordered_map<std::string, MeshDataRenderer> m_Meshes;
		std::unordered_map<std::string, RenderData> m_RendererMeshes;
		std::unordered_map<UUID, Ref<Texture>> m_Textures;
		std::map<UUID, Ref<TextureData>> m_TexturesData;
		std::vector<std::string> m_LoadedMeshNames;
		std::vector<const char*> m_CStrLoadedMeshNames;
	};

	class AssetManager
	{
	private:
		static Unique<Data> s_Data;

	public:
		static MeshDataRenderer& LoadMesh(const std::string& path);
		static MeshDataRenderer& GetMesh(const std::string & AssetName);
		static RenderData LoadRenderData(const std::string& AssetName);
		static void LoadTexture(const UUID& id, const  Ref<Texture>& texture);
		static void LoadTextureData(const UUID& id, const Ref<TextureData>& texture);
		static Ref<Texture>& GetTexture(const UUID& id);
		static Ref<TextureData>& GetTextureData(const UUID& id);
		static void Init();

		static std::vector<std::string>& LoadedMeshes();
		static std::vector<const char*>& CStrLoadedMeshes();

	};
}
