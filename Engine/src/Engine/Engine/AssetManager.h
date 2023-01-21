#pragma once
#include "Base.h"
#include "UUID.h"


namespace Polyboid
{
	class Material;
	class Texture;
	class VertexBufferArray;

	struct Data
	{
		std::unordered_map<std::string, std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>>> m_Meshes;
		std::unordered_map<UUID, Ref<Texture>> m_Textures;
		std::vector<std::string> m_LoadedMeshNames;
		std::vector<const char*> m_CStrLoadedMeshNames;
	};

	class AssetManager
	{
	private:
		static Unique<Data> s_Data;

	public:
		static std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>>& LoadMesh(const std::string& path);
		static std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>>& GetMesh(const std::string & AssetName);
		static void LoadTexture(const UUID& id, const  Ref<Texture>& texture);
		static Ref<Texture>& GetTexture(const UUID& id);
		static void Init();

		static std::vector<std::string>& LoadedMeshes();
		static std::vector<const char*>& CStrLoadedMeshes();

	};
}
