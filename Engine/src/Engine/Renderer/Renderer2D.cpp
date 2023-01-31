#include "boidpch.h"

#include "Renderer2D.h"
#include "RenderAPI.h"
#include "imgui.h"

#define GLM_MESSAGES 1
#define GLM_FORCE_AVX2
#define GLM_CONFIG_ALIGNED_GENTYPES 1
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/detail/qualifier.hpp"
#include <Engine/Renderer/UniformBuffer.h>
#include <glm/gtc/type_ptr.hpp>
#include "VertexBufferArray.h"


namespace Polyboid
{
	static const uint32_t MAX_GEOMETRY = 20000;

	struct QuadData
	{
		uint32_t quadsVertsCount = 4;
		uint32_t quadsIndicesCount = 6;

		Ref<VertexBufferArray> QuadVertexBufferArray;
		Ref<VertexBuffer> QuadVertexBuffer;
		Ref<Shader> shader;

		uint32_t quadCount = 0;
		uint32_t quadOffset = 0;


		uint32_t vtxSize = quadsVertsCount * MAX_GEOMETRY;
		uint32_t idxSize = quadsIndicesCount * MAX_GEOMETRY;

		glm::vec4 QuadVertexPositions[4] = {};
		QuadVertex quadVerts[4] = {};


		std::vector<QuadVertex> QuadVerticesData;
		uint32_t quadIndexCount = 0;
	};

	struct LineData
	{
		uint32_t LinesVertsCount = 4;
		uint32_t LinesIndicesCount = 6;

		Ref<VertexBufferArray> LineVertexBufferArray;
		Ref<VertexBuffer> LineVertexBuffer;
		Ref<Shader> shader;

		uint32_t lineCount = 0;
		uint32_t lineOffset = 0;


		uint32_t vtxSize = LinesVertsCount * MAX_GEOMETRY;
		uint32_t idxSize = LinesIndicesCount * MAX_GEOMETRY;

		glm::vec4 LineVertexPositions[4] = {};
		LineVertex LineVerts[4] = {};


		std::vector<LineVertex> LineVerticesData;
		uint32_t LineIndexCount = 0;
	};

	struct CircleData
	{
		uint32_t quadsVertsCount = 4;
		uint32_t quadsIndicesCount = 6;

		Ref<VertexBufferArray> CircleVertexBufferArray;
		Ref<VertexBuffer> CircleVertexBuffer;
		Ref<Shader> shader;

		uint32_t circleCount = 0;
		uint32_t circleOffset = 0;


		uint32_t vtxSize = quadsVertsCount * MAX_GEOMETRY;
		uint32_t idxSize = quadsIndicesCount * MAX_GEOMETRY;

		glm::vec4 circleVertexPositions[4] = {};
		CircleVertex circleVerts[4] = {};


		std::vector<CircleVertex> CircleVerticesData;

		uint32_t circleIndexCount = 0;
	};

	struct Renderer2DData
	{
		
		std::shared_ptr<UniformBuffer> m_CameraUB;
	};

	static Renderer2DData s_RenderData;
	static QuadData s_QuadData;
	static CircleData s_CircleData;
	static LineData s_LineData;

	void Renderer2D::PrepareQuads()
	{
		POLYBOID_PROFILE_FUNCTION();

		s_QuadData.QuadVertexBufferArray = VertexBufferArray::MakeVertexBufferArray();

		uint32_t vtxSize = s_QuadData.vtxSize;
		uint32_t idxSize = s_QuadData.idxSize;

		s_QuadData.QuadVertexBuffer = VertexBuffer::MakeVertexBuffer(vtxSize * sizeof(QuadVertex));
		s_QuadData.QuadVerticesData = std::vector<QuadVertex>(vtxSize);
		auto* quadIndices = new uint32_t[idxSize];

		uint32_t lastIndex = 0;

		for (uint32_t i = 0; i < idxSize; i)
		{
			if (lastIndex == 0)
			{
				quadIndices[i++] = 0;
				quadIndices[i++] = 1;
				quadIndices[i++] = 3;

				quadIndices[i++] = 1;
				quadIndices[i++] = 2;
				quadIndices[i++] = 3;
			}
			else
			{
				quadIndices[i++] = quadIndices[lastIndex - 6] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 5] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 4] + 4;

				quadIndices[i++] = quadIndices[lastIndex - 3] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 2] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 1] + 4;
			}


			lastIndex = i;
		}


		s_QuadData.QuadVertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::MakeIndexBuffer(quadIndices, idxSize);
		delete[] quadIndices;

		s_QuadData.QuadVertexBuffer->DescribeBuffer({
			{BufferComponent::Float3, "aPosition"},
			{BufferComponent::Float4, "aColor"},
			{BufferComponent::Float2, "aUV"}
		});

		s_QuadData.QuadVertexBufferArray->AddVertexBuffer(s_QuadData.QuadVertexBuffer);
		s_QuadData.QuadVertexBufferArray->SetIndexBuffer(indexBuffer);


		s_QuadData.quadVerts[0] = {{0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
		s_QuadData.quadVerts[1] = {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
		s_QuadData.quadVerts[2] = {{-0.5f, -0.5f, 0.0f,}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};
		s_QuadData.quadVerts[3] = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};

		s_QuadData.shader = Shader::MakeShader("Assets/Shaders/rendererQuad.vert",
			"Assets/Shaders/rendererQuad.frag");

	}


	void Renderer2D::PrepareCircles()
	{
		s_CircleData.CircleVertexBufferArray = VertexBufferArray::MakeVertexBufferArray();

		const uint32_t vtxSize = s_CircleData.vtxSize;
		const uint32_t idxSize = s_CircleData.idxSize;

		s_CircleData.CircleVertexBuffer = VertexBuffer::MakeVertexBuffer(vtxSize * sizeof(CircleVertex));
		s_CircleData.CircleVerticesData = std::vector<CircleVertex>(vtxSize);
		auto* quadIndices = new uint32_t[idxSize];

		uint32_t lastIndex = 0;

		for (uint32_t i = 0; i < idxSize; i)
		{
			if (lastIndex == 0)
			{
				quadIndices[i++] = 0;
				quadIndices[i++] = 1;
				quadIndices[i++] = 3;

				quadIndices[i++] = 1;
				quadIndices[i++] = 2;
				quadIndices[i++] = 3;
			}
			else
			{
				quadIndices[i++] = quadIndices[lastIndex - 6] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 5] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 4] + 4;

				quadIndices[i++] = quadIndices[lastIndex - 3] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 2] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 1] + 4;
			}


			lastIndex = i;
		}


		s_CircleData.CircleVertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::MakeIndexBuffer(quadIndices, idxSize);
		delete[] quadIndices;


		s_CircleData.CircleVertexBuffer->DescribeBuffer({
			{BufferComponent::Float3, "aWorldPosition"},
			{BufferComponent::Float3, "aLocalPosition"},
			{BufferComponent::Float4, "aColor"},
			{BufferComponent::Float, "aThickness"},
			{BufferComponent::Float, "aFade"},

		});

		s_CircleData.CircleVertexBufferArray->AddVertexBuffer(s_CircleData.CircleVertexBuffer);
		s_CircleData.CircleVertexBufferArray->SetIndexBuffer(indexBuffer);


		s_CircleData.circleVerts[0] = {{0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1, 1};
		s_CircleData.circleVerts[1] = {{0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 0.0f};
		s_CircleData.circleVerts[2] = {{-0.5f, -0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f,} , {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 1.0f};
		s_CircleData.circleVerts[3] = {{-0.5f, 0.5f, 0.0f},{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 1.0f};

		s_CircleData.shader = Shader::MakeShader("Assets/Shaders/rendererCircle.vert",
			"Assets/Shaders/rendererCircle.frag");
	}

	void Renderer2D::PrepareLines()
	{
		s_LineData.LineVertexBufferArray = VertexBufferArray::MakeVertexBufferArray();

		const uint32_t vtxSize = s_LineData.vtxSize;
		const uint32_t idxSize = s_LineData.idxSize;

		s_LineData.LineVertexBuffer = VertexBuffer::MakeVertexBuffer(vtxSize * sizeof(LineVertex));
		s_LineData.LineVerticesData = std::vector<LineVertex>(vtxSize);
		auto* quadIndices = new uint32_t[idxSize];

		uint32_t lastIndex = 0;

		for (uint32_t i = 0; i < idxSize; i)
		{
			if (lastIndex == 0)
			{
				quadIndices[i++] = 0;
				quadIndices[i++] = 1;
				quadIndices[i++] = 3;

				quadIndices[i++] = 1;
				quadIndices[i++] = 2;
				quadIndices[i++] = 3;
			}
			else
			{
				quadIndices[i++] = quadIndices[lastIndex - 6] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 5] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 4] + 4;

				quadIndices[i++] = quadIndices[lastIndex - 3] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 2] + 4;
				quadIndices[i++] = quadIndices[lastIndex - 1] + 4;
			}


			lastIndex = i;
		}


		s_LineData.LineVertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::MakeIndexBuffer(quadIndices, idxSize);
		delete[] quadIndices;


		s_LineData.LineVertexBuffer->DescribeBuffer({
			{BufferComponent::Float3, "aWorldPosition"},
			{BufferComponent::Float3, "aLocalPosition0"},
			{BufferComponent::Float3, "aLocalPosition1"},
			{BufferComponent::Float4, "aColor"},
			{BufferComponent::Float, "aThickness"},
			{BufferComponent::Float2, "aUV"}
			});

		s_LineData.LineVertexBufferArray->AddVertexBuffer(s_LineData.LineVertexBuffer);
		s_LineData.LineVertexBufferArray->SetIndexBuffer(indexBuffer);


		s_LineData.LineVerts[0] = { {0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, {1.0f, 1.0f}};
		s_LineData.LineVerts[1] = { {0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, {1.0f, 0.0f}};
		s_LineData.LineVerts[2] = { {-0.5f, -0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f},{-0.5f, -0.5f, 0.0f} , {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f , {0.0f, 0.0f}};
		s_LineData.LineVerts[3] = { {-0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, {0.0f, 1.0f}};

		s_LineData.shader = Shader::MakeShader("Assets/Shaders/rendererLine.vert",
			"Assets/Shaders/rendererLine.frag");
	}

	void Renderer2D::PrepareQuadsForRendering()
	{
		if (s_QuadData.quadCount)
		{
			

			uint32_t vertexSize = sizeof(QuadVertex) * s_QuadData.quadOffset;
			s_QuadData.QuadVertexBuffer->SetData(s_QuadData.QuadVerticesData.data(), vertexSize);

			s_QuadData.shader->Bind();
			const auto count = s_QuadData.quadIndexCount;

			s_QuadData.QuadVertexBufferArray->Bind();
			s_QuadData.QuadVertexBuffer->Bind();
			s_QuadData.QuadVertexBufferArray->GetIndexBuffer()->Bind();

			glDisable(GL_CULL_FACE);
			RenderAPI::DrawIndexed(count, 4);
			glEnable(GL_CULL_FACE);
		}
	}


	void Renderer2D::PrepareCircleForRendering()
	{
		if (s_CircleData.circleCount)
		{
			

			const uint32_t vertexSize = sizeof(CircleVertex) * s_CircleData.circleOffset;
			s_CircleData.CircleVertexBuffer->SetData(s_CircleData.CircleVerticesData.data(), vertexSize);

			s_CircleData.shader->Bind();
			const auto count = s_CircleData.circleIndexCount;

			s_CircleData.CircleVertexBufferArray->Bind();
			s_CircleData.CircleVertexBuffer->Bind();
			s_CircleData.CircleVertexBufferArray->GetIndexBuffer()->Bind();

			glDisable(GL_CULL_FACE);
			RenderAPI::DrawIndexed(count, 4);
			glEnable(GL_CULL_FACE);
		}
	}

	void Renderer2D::PrepareLineForRendering()
	{
		if (s_LineData.lineCount)
		{


			const uint32_t vertexSize = sizeof(LineVertex) * s_LineData.lineOffset;
			s_LineData.LineVertexBuffer->SetData(s_LineData.LineVerticesData.data(), vertexSize);

			s_LineData.shader->Bind();
			const auto count = s_LineData.LineIndexCount;

			s_LineData.LineVertexBufferArray->Bind();
			s_LineData.LineVertexBuffer->Bind();
			s_LineData.LineVertexBufferArray->GetIndexBuffer()->Bind();

			glDisable(GL_CULL_FACE);
			RenderAPI::DrawIndexed(count, 4);
			glEnable(GL_CULL_FACE);
		}
	}


	void Renderer2D::Init()
	{
		POLYBOID_PROFILE_FUNCTION();
		PrepareQuads();
		PrepareCircles();
		PrepareLines();

		//temp solution
		s_RenderData.m_CameraUB = UniformBuffer::MakeUniformBuffer(sizeof(glm::mat4) * 2, 0);
	}

	void Renderer2D::BeginDraw(const Ref<Camera>& camera)
	{
		POLYBOID_PROFILE_FUNCTION();

		s_RenderData.m_CameraUB->SetData(glm::value_ptr(camera->GetProjection()), sizeof(glm::mat4));
		s_RenderData.m_CameraUB->SetData(glm::value_ptr(camera->GetView()), sizeof(glm::mat4), 64);
	}

	void Renderer2D::DebugWindow()
	{
		POLYBOID_PROFILE_FUNCTION();
	}

	void Renderer2D::EndDraw()
	{
		POLYBOID_PROFILE_FUNCTION();

		PrepareQuadsForRendering();
		PrepareCircleForRendering();
		PrepareLineForRendering();

		Reset();
	}



	void Renderer2D::ResetQuads()
	{
		s_QuadData.quadCount = 0;
		s_QuadData.quadOffset = 0;
		s_QuadData.quadIndexCount = 0;
	}

	void Renderer2D::ResetCircles()
	{
		s_CircleData.circleCount = 0;
		s_CircleData.circleOffset = 0;
		s_CircleData.circleIndexCount = 0;
	}

	void Renderer2D::ResetLines()
	{
		s_LineData.lineCount = 0;
		s_LineData.lineOffset = 0;
		s_LineData.LineIndexCount = 0;
	}

	void Renderer2D::Reset()
	{
		POLYBOID_PROFILE_FUNCTION();
		ResetQuads();
		ResetCircles();
		ResetLines();
	}

	void Renderer2D::Shutdown()
	{
		POLYBOID_PROFILE_FUNCTION();

		if (!s_QuadData.QuadVerticesData.empty())
		{
			s_QuadData.QuadVerticesData.clear();
		}
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		POLYBOID_PROFILE_FUNCTION();

		int count = 0;
		for (auto& quadVert : s_QuadData.quadVerts)
		{
			if (s_QuadData.quadOffset < s_QuadData.vtxSize)
			{
				s_QuadData.QuadVerticesData.at(s_QuadData.quadOffset + count).position = transform * glm::vec4(
					quadVert.position, 1.0f);
				s_QuadData.QuadVerticesData.at(s_QuadData.quadOffset + count).color = color;
				s_QuadData.QuadVerticesData.at(s_QuadData.quadOffset + count).uv = quadVert.uv;
			}

			count++;
		}


		if (s_QuadData.quadOffset < s_QuadData.vtxSize)
		{
			s_QuadData.quadCount++;
			s_QuadData.quadOffset += 4;
			s_QuadData.quadIndexCount += 6;
		}
	}

	void Renderer2D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, float thickness)
	{
		int count = 0;
		for (auto& lineVert : s_LineData.LineVerts)
		{
			if (s_LineData.lineOffset < s_LineData.vtxSize)
			{
				glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(glm::length(p1 - p0)));
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).WorldPosition = transform * glm::vec4(lineVert.WorldPosition, 1.0f);
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).LocalPosition0 = p0;
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).LocalPosition1 = p1;
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).color = color;
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).Thickness = thickness;
				s_LineData.LineVerticesData.at(s_LineData.lineOffset + count).uv = lineVert.uv;

			}

			count++;
		}

		if (s_LineData.lineOffset < s_LineData.vtxSize)
		{
			s_LineData.lineCount++;
			s_LineData.lineOffset += 4;
			s_LineData.LineIndexCount += 6;
		}
	}

	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		int count = 0;
		for (auto& circleVert : s_CircleData.circleVerts)
		{
			if (s_CircleData.circleOffset < s_CircleData.vtxSize)
			{
				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).WorldPosition = transform * glm::vec4(circleVert.LocalPosition, 1.0f);

				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).LocalPosition =  circleVert.LocalPosition;
				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).color = color;
				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).thickness = thickness;
				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).fade = fade;
			}

			count++;
		}

		if (s_CircleData.circleOffset < s_CircleData.vtxSize)
		{
			s_CircleData.circleCount++;
			s_CircleData.circleOffset += 4;
			s_CircleData.circleIndexCount += 6;
		}
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec4& color)
	{
		POLYBOID_PROFILE_FUNCTION();

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);

		DrawQuad(translate, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color)
	{
		POLYBOID_PROFILE_FUNCTION();

		const auto translate = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
			glm::mat4(1.0f), glm::radians(rotation),
			glm::vec3(0, 0, 1));

		DrawQuad(translate, color);
	}
}
