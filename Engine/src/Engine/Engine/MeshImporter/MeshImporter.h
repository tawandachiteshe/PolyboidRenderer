#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <filesystem>
#include <map>
#include <tuple>

#include "Engine/Engine/Math/AABB.h"


struct aiMaterial;
struct aiNode;
struct aiScene;
struct aiMesh;

namespace Polyboid
{
	class UUID;
	struct MaterialData;
	class ShaderBufferStorage;
	class Material;
	class VertexBufferArray;

	struct RendererVertex
	{

		RendererVertex() = default;
		glm::vec3 position;
		glm::vec3 normals;
		glm::vec2 uv;
		glm::vec3 tangents;
		glm::vec3 bitangents;
		float meshIdx;
	};

	

	struct RendererMeshData
	{

		//data we want so farrr..

		RendererMeshData() = default;
		std::vector<RendererVertex> vertices;
		std::vector<uint32_t> Indices;
		AABB BoundingBox;

		
	};


	using MeshData = std::map<Ref<Material>, std::vector<RendererMeshData>>;
	using MeshDataRenderer = std::map<Ref<Material>, std::pair<Ref<VertexBufferArray>, std::vector<AABB>>>;

	class MeshImporter
	{
	private:
		static  RendererMeshData GetMesh(const aiMesh* mesh, uint32_t meshIdx);
		static  void ProcessNode(aiNode* node, const aiScene* scene, MeshData& meshesData, const std::filesystem::path& path);
		

	public:
		static MeshData Read(const std::filesystem::path& path);
		static MeshDataRenderer ReadForRendering(const std::filesystem::path& path);

	};
	
}
