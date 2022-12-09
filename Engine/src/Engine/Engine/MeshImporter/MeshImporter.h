#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include <filesystem>


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
	public:
		static RendererMeshData Read(const std::filesystem::path& path);
		static Ref<VertexBufferArray> ReadForRendering(const std::filesystem::path& path);

	};
	
}
