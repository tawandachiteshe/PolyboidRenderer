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

	struct LineVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition0;
		glm::vec3 LocalPosition1;
		glm::vec4 color;
		float Thickness;
		glm::vec2 uv;
		

		LineVertex() = default;

		LineVertex(const glm::vec3& world_position, const glm::vec3& local_position0, const glm::vec3& local_position1,
			const glm::vec4& color, float thickness, const glm::vec2& uv)
			: WorldPosition(world_position),
			  LocalPosition0(local_position0),
			  LocalPosition1(local_position1),
			  color(color),
		      Thickness(thickness),
			  uv(uv)
		{
		}
	};



	class Renderer2D
	{
	private:

		static void PrepareQuads();
		static void PrepareCircles();
		static void PrepareLines();
		static void PrepareQuadsForRendering();
		static void PrepareCircleForRendering();
		static void PrepareLineForRendering();

		static void ResetQuads();
		static void ResetCircles();
		static void ResetLines();

	public:
		static void Init();
		static void BeginDraw(const Ref<Camera>& camera);
		static void DebugWindow();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f });

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4( 1.0f ), float thickness = 0, float fade = 0);
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color = glm::vec4(1.0f), float thickness = 0.1f);

		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4{ 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color = glm::vec4{ 1.0f });
		static void EndDraw();
		static void Reset();
		static void Shutdown();
	};


}

