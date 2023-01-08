#pragma once
#include "Base.h"
#include "UUID.h"


namespace Polyboid
{
	class VertexBufferArray;

	struct Data
	{
		std::unordered_map<std::string, std::vector<Ref<VertexBufferArray>>> m_Meshes;
		std::vector<std::string> m_LoadedMeshNames;
		std::vector<const char*> m_CStrLoadedMeshNames;
	};

	class AssetManager
	{
	private:
		static Unique<Data> s_Data;

	public:
		static std::vector<Ref<VertexBufferArray>>& LoadMesh(const std::string& path);
		static std::vector<Ref<VertexBufferArray>>& GetMesh(const std::string & AssetName);
		static void Init();

		static std::vector<std::string>& LoadedMeshes();
		static std::vector<const char*>& CStrLoadedMeshes();

	};
}
