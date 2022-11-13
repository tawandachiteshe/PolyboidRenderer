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

	struct Renderer2DData
	{
		uint32_t quadsVertsCount = 4;
		uint32_t quadsIndicesCount = 6;

		uint32_t maxQuads = 20000;


		uint32_t vtxSize = quadsVertsCount * maxQuads;
		uint32_t idxSize = quadsIndicesCount * maxQuads;

		Ref<VertexBufferArray> vertexBufferArray;
		Ref<VertexBuffer> vertexBuffer;
		Ref<Shader> shader;

		std::vector<Vertex2D> QuadVertexBufferBase;
		Vertex2D* QuadVertexBufferPtr = nullptr;

		uint32_t quadCount = 0;
		uint32_t quadOffset = 0;

		uint32_t indexCount = 0;
		bool submitOnce = false;

		glm::vec4 QuadVertexPositions[4] = {};
		Vertex2D quadVerts[4] = {};

		std::shared_ptr<UniformBuffer> m_CameraUB;


	};

	static Renderer2DData s_sData;

	void Renderer2D::Init()
	{

		s_sData.vertexBufferArray = VertexBufferArray::MakeVertexBufferArray();
		s_sData.shader = Shader::MakeShader("Assets/Shaders/renderer2D.vert",
			"Assets/Shaders/renderer2D.frag");

		uint32_t vtxSize = s_sData.vtxSize;
		uint32_t idxSize = s_sData.idxSize;

		s_sData.vertexBuffer = VertexBuffer::MakeVertexBuffer(vtxSize * sizeof(Vertex2D));

		s_sData.QuadVertexBufferBase = std::vector<Vertex2D>(vtxSize);
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


		s_sData.vertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::MakeIndexBuffer(quadIndices, idxSize);
		delete[] quadIndices;

		s_sData.vertexBuffer->DescribeBuffer({
			{ BufferComponent::Float3, "aPosition" },
			{ BufferComponent::Float4, "aColor" },
			{ BufferComponent::Float2, "aUV" }
			});

		s_sData.vertexBufferArray->AddVertexBuffer(s_sData.vertexBuffer);
		s_sData.vertexBufferArray->SetIndexBuffer(indexBuffer);


		s_sData.quadVerts[0] = { {0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f} };
		s_sData.quadVerts[1] = { { 0.5f, -0.5f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 0.0f } };
		s_sData.quadVerts[2] = { { -0.5f, -0.5f, 0.0f, }, {1.0f, 1.0f, 1.0f, 1.0f}, { 0.0f, 1.0f } };
		s_sData.quadVerts[3] = { { -0.5f, 0.5f, 0.0f }, {1.0f, 1.0f, 1.0f, 1.0f}, { 1.0f, 1.0f } };

		//temp solution
		s_sData.m_CameraUB = UniformBuffer::MakeUniformBuffer(sizeof(glm::mat4) * 2, 0);

	}

	void Renderer2D::BeginDraw(const Ref<Camera>& camera)
	{

		s_sData.m_CameraUB->SetData(glm::value_ptr(camera->GetProjection()), sizeof(glm::mat4));
		s_sData.m_CameraUB->SetData(glm::value_ptr(camera->GetView()), sizeof(glm::mat4), 64);

		s_sData.QuadVertexBufferPtr = s_sData.QuadVertexBufferBase.data();

	}

	void Renderer2D::DebugWindow()
	{

		ImGui::Begin("Renderer 2D stats");
		ImGui::Text("Quad count: %d ", s_sData.quadCount);
		ImGui::Text("Triangle count: %d ", s_sData.quadCount * 2);
		ImGui::Text("Quad offset: %d ", s_sData.quadOffset);
		ImGui::Text("Draw Index count: %d", s_sData.indexCount);
		

		ImGui::Text("Renderer 2D current vtx bytes: %d ", s_sData.quadOffset * sizeof(Vertex2D));
		ImGui::Text("Renderer 2D current idx bytes: %d ", s_sData.indexCount * sizeof(uint32_t));
		ImGui::Text("Renderer 2D total vtx bytes: %d ", s_sData.vtxSize * sizeof(Vertex2D));
		ImGui::Text("Renderer 2D total idx bytes: %d ", s_sData.idxSize * sizeof(uint32_t));

		ImGui::End();
	}

	void Renderer2D::EndDraw()
	{
		
		if (s_sData.quadCount)
		{
			
			s_sData.vertexBufferArray->Bind();

			uint32_t vertexSize = sizeof(Vertex2D) * s_sData.quadOffset;
			s_sData.vertexBuffer->SetData(s_sData.QuadVertexBufferBase.data(), vertexSize);

			s_sData.submitOnce = true;
			s_sData.shader->Bind();
			const auto count = s_sData.indexCount;

			s_sData.vertexBuffer->Bind();
			s_sData.vertexBufferArray->GetIndexBuffer()->Bind();
			RenderAPI::DrawIndexed(count, 4);
			
		}
		


		Reset();
	}

	void Renderer2D::Reset()
	{
		s_sData.quadCount = 0;
		s_sData.quadOffset = 0;
		s_sData.indexCount = 0;
	}

	void Renderer2D::Shutdown()
	{
		if (!s_sData.QuadVertexBufferBase.empty())
		{
			s_sData.QuadVertexBufferBase.clear();
		}
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{

		
		for (auto& quadVert : s_sData.quadVerts)
		{
			if (s_sData.quadOffset < s_sData.vtxSize)
			{

				s_sData.QuadVertexBufferPtr->position = transform * glm::vec4(quadVert.position, 1.0f);
				s_sData.QuadVertexBufferPtr->color = color;
				s_sData.QuadVertexBufferPtr->uv = quadVert.uv;
				s_sData.QuadVertexBufferPtr++;

			}

		}
		

		if (s_sData.quadOffset < s_sData.vtxSize)
		{
			s_sData.quadCount++;
			s_sData.quadOffset += 4;
			s_sData.indexCount += 6;
		}
		
		
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec4& color)
	{

		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);

		DrawQuad(translate, color);

	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color)
	{

		const auto translate = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation),
			glm::vec3(0, 0, 1));

		DrawQuad(translate, color);

	}
}


