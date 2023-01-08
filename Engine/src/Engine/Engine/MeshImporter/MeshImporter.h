#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <filesystem>


struct aiNode;
struct aiScene;
struct aiMesh;

namespace Polyboid
{
	class VertexBufferArray;

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
		static  RendererMeshData GetMesh(const aiMesh* mesh);
		static  void ProcessNode(aiNode* node, const aiScene* scene, std::vector<RendererMeshData>& meshesData);
		

	public:
		static std::vector<RendererMeshData> Read(const std::filesystem::path& path);
		static std::vector<Ref<VertexBufferArray>> ReadForRendering(const std::filesystem::path& path);

	};
	
}
