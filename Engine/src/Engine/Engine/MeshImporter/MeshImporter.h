#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <filesystem>
#include <map>
#include <tuple>


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

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec4> colors;
	};

	

	struct RendererMeshData
	{

		//data we want so farrr..

		RendererMeshData() = default;

		bool HasNormals = false;
		bool HasColors = false;
		RendererVertex vertex;
		std::vector<float> vertices;
		std::vector<uint32_t> Indices;


		
	};


	using MeshData = std::map<Ref<Material>, std::vector<RendererMeshData>>;
	using MeshDataRenderer = std::map<Ref<Material>, Ref<VertexBufferArray>>;

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
