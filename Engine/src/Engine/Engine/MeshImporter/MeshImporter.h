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


	
	class MeshImporter
	{


	};
	
}
