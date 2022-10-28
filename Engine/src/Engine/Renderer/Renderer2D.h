#pragma once
#include "Camera3D.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/IndexBuffer.h"
#include "Engine/Renderer/Shader.h"
#include "Engine/Renderer/VertexBuffer.h"
#include "Engine/Renderer/VertexBufferArray.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
	struct Vertex2D
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;

		Vertex2D() = default;

		Vertex2D(const glm::vec3& position, const glm::vec4& color, const glm::vec2& uv) : position(position), color(color), uv(uv) {}

	};



	class Renderer2D
	{

	public:
		static void Init();
		static void BeginDraw(const Ref<Camera3D>& camera);
		static void DebugWindow();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4{ 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color = glm::vec4{ 1.0f });
		static void EndDraw();
		static void Reset();
		static void Shutdown();
	};

}

