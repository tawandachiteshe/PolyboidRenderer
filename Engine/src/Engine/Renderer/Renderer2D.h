#pragma once
#include "Camera3D.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Math/AABB.h"
#include "Engine/Renderer/Shader.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
	class RenderAPI;
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
		glm::vec3 Position;
		glm::vec4 color;
		

		LineVertex() = default;

		LineVertex(const glm::vec3& position, const glm::vec4& color)
			: Position(position),
			  color(color)
		{
		}
	};

	using Render2DShaderType = std::pair<Ref<Shader>, Ref<Shader>>;

	class Renderer2D
	{
	private:

		static void PrepareQuads();
		static void PrepareCircles();
		static void PrepareLines();
		static void PrepareQuadsForRendering();
		static void PrepareCircleForRendering();
		static void PrepareLineForRendering();

		//This is an internal thing it won't load your custom shaders
		static Render2DShaderType  LoadShader(const std::string& shaderName);

		static void ResetQuads();
		static void ResetCircles();
		static void ResetLines();

	public:
		struct Renderer2DCameraData
		{
			Renderer2DCameraData() = default;
			glm::mat4 projection;
			glm::mat4 view;
			glm::vec3 pos;
		};

		static void Init(RenderAPI* context);
		static void BeginDraw(const Ref<Camera>& camera);
		static void DebugWindow();
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color = glm::vec4{ 1.0f });

		static void DrawCircle(const glm::mat4& transform, const glm::vec4& color = glm::vec4( 1.0f ), float thickness = .5f, float fade = 0);
		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color = glm::vec4(1.0f), float thickness = 0.1f);
		static void DrawRect(const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color = glm::vec4{1.0f}, float thickness = 0.1f);
		static  void DrawRect(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static  void DrawCube(const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static  void DrawCube(const glm::mat4& transform, const glm::vec3& extends ,const glm::vec4& color = glm::vec4(1.0f));
		static  void DrawCube(const glm::mat4& transform, const AABB& aabb, const glm::vec4& color = glm::vec4(1.0f));

		static  void DrawPyramid(const glm::mat4& transform, float farPlane = 1.0f, float nearPlane = 1.0f, float distance = 1.0f,const glm::vec4& color = glm::vec4(1.0f));

		static  void DrawCameraFrustum(const Ref<Camera>& camera, const glm::vec4& color = glm::vec4(1.0f));


		static void DrawQuad(const glm::vec3& position, const glm::vec4& color = glm::vec4{ 1.0f });
		static void DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color = glm::vec4{ 1.0f });
		static void EndDraw();
		static void Reset();
		static void Shutdown();
	};


}

