#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <filesystem>


struct aiMaterial;
struct aiNode;
struct aiScene;
struct aiMesh;

namespace Polyboid
{
	class Material;
	class VertexBufferArray;

	using MeshDataIdxArray = std::vector<size_t>;
	using MatToMeshMap = std::map<aiMaterial*, MeshDataIdxArray*>;

	struct RendererVertex
	{

		RendererVertex() = default;

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;
	};

	

	struct RendererMeshData
	{

		//data we want so farrr..

		RendererMeshData() = default;

		bool HasNormals;
		bool HasColors;
		RendererVertex vertex;
		std::vector<float> vertices;
		std::vector<uint32_t> Indices;


		
	};

	class MeshImporter
	{
	private:
		static  RendererMeshData GetMeshV2(const aiScene* scene, const MatToMeshMap& meshToMap);
		static  RendererMeshData GetMesh(const aiMesh* mesh);
		static  void ProcessNode(aiNode* node, const aiScene* scene, std::vector<std::pair<RendererMeshData, Ref<Material>>>& meshesData);
		

	public:
		static std::vector<std::pair<RendererMeshData, Ref<Material>>> Read(const std::filesystem::path& path);
		static std::vector<std::pair<Ref<VertexBufferArray>, Ref<Material>>> ReadForRendering(const std::filesystem::path& path);

	};
	
}
