#pragma once
#include "Camera3D.h"
#include "Engine/Engine/Base.h"
#include "Engine/Renderer/Shader.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
	class EditorCamera;


	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 color;
		float thickness;
		float fade;

		CircleVertex() = default;

		CircleVertex(const glm::vec3& world_position, const glm::vec3& local_position, const glm::vec4& color,
		float thickness, float fade)
			: WorldPosition(world_position),
			  LocalPosition(local_position),
			  color(color),
			  thickness(thickness),
			  fade(fade)
		{
		}
	};

	struct QuadVertex
	{
		glm::vec3 position;
		glm::vec4 color;
		glm::vec2 uv;

		QuadVertex() = default;

		QuadVertex(const glm::vec3& position, const glm::vec4& color, const glm::vec2& uv) : position(position), color(color), uv(uv) {}

	};



	class Renderer2D
	{
	private:

		static void PrepareQuads();
		static void PrepareCircles();
		static void PrepareQuadsForRendering();
		static void PrepareCircleForRendering();

		static void ResetQuads();
		static void ResetCircles();

	public:
		static void Init();
		static void BeginDraw(const Ref<Camera>& camera);
		static void DebugWindow();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f });

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4( 1.0f ), float thickness = 0, float fade = 0);

		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4{ 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color = glm::vec4{ 1.0f });
		static void EndDraw();
		static void Reset();
		static void Shutdown();
	};

}

