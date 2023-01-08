#pragma once
#include "Engine/Engine/Base.h"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"


namespace Polyboid
{
	class VertexBufferArray;

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 uv;
	};

	class Primitives
	{
	public:
		static Ref<VertexBufferArray> GenQuad();
	};
}
