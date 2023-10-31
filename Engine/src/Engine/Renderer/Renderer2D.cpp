#include "boidpch.h"

#include "Renderer2D.h"

#include <mutex>

#include "RenderAPI.h"
#include "imgui.h"
#include <glm/glm.hpp>
#include "glm/ext/matrix_transform.hpp"
#include "glm/detail/qualifier.hpp"
#include <Engine/Renderer/UniformBuffer.h>
#include <glm/gtc/type_ptr.hpp>

#include "FrustumCulling.h"
#include "GraphicsPipeline.h"
#include "RasterizerState.h"
#include "RenderCommand.h"
#include "VertexBufferArray.h"


namespace Polyboid
{
	static const uint32_t MAX_GEOMETRY = 20000;

	struct QuadData
	{
		uint32_t quadsVertsCount = 4;
		uint32_t quadsIndicesCount = 6;

		Ref<VertexBufferArray> QuadVertexBufferArray;
		Ref<VertexBufferSet> QuadVertexBuffer;
		Ref<StagingBufferSet> QuadVerticesBuffer;

		uint32_t quadCount = 0;
		uint32_t quadOffset = 0;


		uint32_t vtxSize = quadsVertsCount * MAX_GEOMETRY;
		uint32_t idxSize = quadsIndicesCount * MAX_GEOMETRY;

		glm::vec4 QuadVertexPositions[4] = {};
		QuadVertex quadVerts[4] = {};


		std::vector<QuadVertex> QuadVerticesData;
		uint32_t quadIndexCount = 0;

		Ref<GraphicsPipeline> QuadPipelineState;
	};

	struct LineData
	{
		uint32_t LinesVertsCount = 2;

		Ref<VertexBufferArray> LineVertexBufferArray;
		Ref<VertexBufferSet> LineVertexBuffer;
		Ref<StagingBufferSet> LineVerticesBuffer;

		uint32_t lineCount = 0;
		uint32_t lineOffset = 0;
		uint32_t vtxSize = LinesVertsCount * MAX_GEOMETRY;


		std::vector<LineVertex> LineVerticesData;
		LineVertex lineVerts[4];

		uint32_t LineIndexCount = 0;

		Ref<GraphicsPipeline> LinePipelineState;
	};

	struct CircleData
	{
		uint32_t quadsVertsCount = 4;
		uint32_t quadsIndicesCount = 6;

		Ref<VertexBufferArray> CircleVertexBufferArray;
		Ref<VertexBufferSet> CircleVertexBuffer;
		Ref<StagingBufferSet> CircleVerticesBuffer;

		uint32_t circleCount = 0;
		uint32_t circleOffset = 0;


		uint32_t vtxSize = quadsVertsCount * MAX_GEOMETRY;
		uint32_t idxSize = quadsIndicesCount * MAX_GEOMETRY;

		glm::vec4 circleVertexPositions[4] = {};
		CircleVertex circleVerts[4] = {};


		std::vector<CircleVertex> CircleVerticesData;
		Ref<GraphicsPipeline> CirclePipelineState;

		uint32_t circleIndexCount = 0;
	};

	struct Renderer2DData
	{
		Ref<UniformBufferSet> m_CameraUB;
		Ref<StagingBufferSet> m_CameraUBData;
		Ref<Camera> m_Renderer2DCamera;
		std::mutex m_CameraDataMutex;
		Ref<RenderPass> m_RenderPass;
		Ref<CommandBufferSet> m_UploadCommands;

		Renderer2D::Renderer2DCameraData* m_CameraData = new Renderer2D::Renderer2DCameraData;
	};

	static Renderer2DData s_RenderData;
	static QuadData s_QuadData;
	static CircleData s_CircleData;
	static LineData s_LineData;

	void Renderer2D::PrepareQuads()
	{
		s_QuadData.QuadVertexBufferArray = VertexBufferArray::Create();

		uint32_t vtxSize = s_QuadData.vtxSize;
		uint32_t idxSize = s_QuadData.idxSize;

		auto vertBytesSize = static_cast<uint32_t>(vtxSize * sizeof(QuadVertex));

		s_QuadData.QuadVertexBuffer = VertexBufferSet::Create(vertBytesSize);
		s_QuadData.QuadVerticesBuffer = StagingBufferSet::Create(vertBytesSize);
		s_QuadData.QuadVerticesData = std::vector<QuadVertex>(vtxSize);
		auto* quadIndices = new uint32_t[idxSize];

		uint32_t offset = 0;
		for (uint32_t i = 0; i < idxSize; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}


		s_QuadData.QuadVertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, idxSize);
		delete[] quadIndices;

		s_QuadData.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aColor"},
			{ShaderDataType::Float2, "aUV"}
		});

		s_QuadData.QuadVertexBufferArray->AddVertexBufferSet(s_QuadData.QuadVertexBuffer);
		s_QuadData.QuadVertexBufferArray->SetIndexBuffer(indexBuffer);


		s_QuadData.quadVerts[0] = {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}};
		s_QuadData.quadVerts[1] = {{0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}};
		s_QuadData.quadVerts[2] = {{0.5f, 0.5f, 0.0f,}, {1.0f, 1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}};
		s_QuadData.quadVerts[3] = {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}};

		s_QuadData.QuadPipelineState = GraphicsPipeline::Create();
		s_QuadData.QuadPipelineState->SetGraphicsShaders(LoadShader("rendererQuad"));
		s_QuadData.QuadPipelineState->SetRenderPass(s_RenderData.m_RenderPass);
		s_QuadData.QuadPipelineState->SetVertexArray(s_QuadData.QuadVertexBufferArray);
		s_QuadData.QuadPipelineState->Bake();
		s_QuadData.QuadPipelineState->AllocateDescriptorSets();
		s_QuadData.QuadPipelineState->BindUniformBufferSet(0, s_RenderData.m_CameraUB);
		s_QuadData.QuadPipelineState->WriteSetResourceBindings();
	}


	void Renderer2D::PrepareCircles()
	{
		s_CircleData.CircleVertexBufferArray = VertexBufferArray::Create();

		const uint32_t vtxSize = s_CircleData.vtxSize;
		const uint32_t idxSize = s_CircleData.idxSize;

		s_CircleData.CircleVertexBuffer = VertexBufferSet::Create(vtxSize * sizeof(CircleVertex));
		s_CircleData.CircleVerticesBuffer = StagingBufferSet::Create(vtxSize * sizeof(CircleVertex));
		s_CircleData.CircleVerticesData = std::vector<CircleVertex>(vtxSize);
		auto* quadIndices = new uint32_t[idxSize];

		uint32_t lastIndex = 0;

		uint32_t offset = 0;
		for (uint32_t i = 0; i < idxSize; i += 6)
		{
			quadIndices[i + 0] = offset + 0;
			quadIndices[i + 1] = offset + 1;
			quadIndices[i + 2] = offset + 2;

			quadIndices[i + 3] = offset + 2;
			quadIndices[i + 4] = offset + 3;
			quadIndices[i + 5] = offset + 0;

			offset += 4;
		}


		s_CircleData.CircleVertexBufferArray->Bind();
		const Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(quadIndices, idxSize);
		delete[] quadIndices;


		s_CircleData.CircleVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aWorldPosition"},
			{ShaderDataType::Float3, "aLocalPosition"},
			{ShaderDataType::Float4, "aColor"},
			{ShaderDataType::Float, "aThickness"},
			{ShaderDataType::Float, "aFade"},

		});

		s_CircleData.CircleVertexBufferArray->AddVertexBufferSet(s_CircleData.CircleVertexBuffer);
		s_CircleData.CircleVertexBufferArray->SetIndexBuffer(indexBuffer);


		s_CircleData.circleVerts[0] = {{-0.5f, -0.5f, 0.0f}, {-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1, 1};
		s_CircleData.circleVerts[1] = {{0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 0.0f};
		s_CircleData.circleVerts[2] = {{0.5f, 0.5f, 0.0f}, {0.5f, 0.5f, 0.0f,}, {1.0f, 1.0f, 1.0f, 1.0f}, 0.0f, 1.0f};
		s_CircleData.circleVerts[3] = {{-0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, 1.0f, 1.0f};


		s_CircleData.CirclePipelineState = GraphicsPipeline::Create();
		s_CircleData.CirclePipelineState->SetGraphicsShaders(LoadShader("rendererCircle"));
		s_CircleData.CirclePipelineState->SetRenderPass(s_RenderData.m_RenderPass);
		s_CircleData.CirclePipelineState->SetVertexArray(s_CircleData.CircleVertexBufferArray);
		s_CircleData.CirclePipelineState->Bake();

		s_CircleData.CirclePipelineState->AllocateDescriptorSets();
		s_CircleData.CirclePipelineState->BindUniformBufferSet(0, s_RenderData.m_CameraUB);
		s_CircleData.CirclePipelineState->WriteSetResourceBindings();
	}

	void Renderer2D::PrepareLines()
	{
		s_LineData.LineVertexBufferArray = VertexBufferArray::Create();

		const uint32_t vtxSize = s_LineData.vtxSize;

		s_LineData.LineVertexBuffer = VertexBufferSet::Create(vtxSize * sizeof(LineVertex));
		s_LineData.LineVerticesData = std::vector<LineVertex>(vtxSize);
		s_LineData.LineVerticesBuffer = StagingBufferSet::Create(vtxSize * sizeof(LineVertex));

		s_LineData.LineVertexBufferArray->Bind();

		s_LineData.LineVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "aPosition"},
			{ShaderDataType::Float4, "aColor"},
		});

		s_LineData.LineVertexBufferArray->AddVertexBufferSet(s_LineData.LineVertexBuffer);


		s_LineData.lineVerts[0] = {{0.5f, 0.5f, 0.0f}, glm::vec4{1.0f}};
		s_LineData.lineVerts[1] = {{0.5f, -0.5f, 0.0f}, glm::vec4{1.0f}};
		s_LineData.lineVerts[2] = {{-0.5f, -0.5f, 0.0f}, glm::vec4{1.0f}};
		s_LineData.lineVerts[3] = {{-0.5f, 0.5f, 0.0f}, glm::vec4{1.0f}};

		s_LineData.LinePipelineState = GraphicsPipeline::Create();
		s_LineData.LinePipelineState->SetGraphicsShaders(LoadShader("rendererLine"));
		s_LineData.LinePipelineState->SetRenderPass(s_RenderData.m_RenderPass);
		s_LineData.LinePipelineState->SetVertexArray(s_LineData.LineVertexBufferArray);
		s_LineData.LinePipelineState->SetGeometryTopology(PrimitiveType::Lines);
		s_LineData.LinePipelineState->GetRasterizerState().SetLineWidth(2.0f);
		s_LineData.LinePipelineState->Bake();

		s_LineData.LinePipelineState->AllocateDescriptorSets();
		s_LineData.LinePipelineState->BindUniformBufferSet(0, s_RenderData.m_CameraUB);
		s_LineData.LinePipelineState->WriteSetResourceBindings();
	}

	void Renderer2D::PrepareQuadsForRendering()
	{
		if (s_QuadData.quadCount)
		{
			auto pipeLine = s_QuadData.QuadPipelineState;


			const auto count = s_QuadData.quadIndexCount;

			RenderCommand::BindGraphicsPipeline(pipeLine);
			RenderCommand::BindGraphicsDescriptorSets(0, pipeLine->GetDescriptorSets(0));
			RenderCommand::BindVertexBuffer(s_QuadData.QuadVertexBuffer);
			RenderCommand::BindIndexBuffer(s_QuadData.QuadVertexBufferArray->GetIndexBuffer());


			RenderCommand::DrawIndexed(count);
		}
	}


	void Renderer2D::PrepareCircleForRendering()
	{
		if (s_CircleData.circleCount)
		{
			auto pipeLine = s_CircleData.CirclePipelineState;

			const auto count = s_CircleData.circleIndexCount;
			RenderCommand::BindGraphicsDescriptorSets(0, pipeLine->GetDescriptorSets(0));

			RenderCommand::BindGraphicsPipeline(pipeLine);
			RenderCommand::BindVertexBuffer(s_CircleData.CircleVertexBuffer);
			RenderCommand::BindIndexBuffer(s_CircleData.CircleVertexBufferArray->GetIndexBuffer());


			RenderCommand::DrawIndexed(count);
		}
	}

	void Renderer2D::PrepareLineForRendering()
	{
		if (s_LineData.lineCount)
		{
			auto pipeLine = s_LineData.LinePipelineState;


			RenderCommand::BindGraphicsPipeline(pipeLine);
			RenderCommand::BindGraphicsDescriptorSets(0, pipeLine->GetDescriptorSets(0));
			RenderCommand::LineWidth(2.0);
			RenderCommand::BindVertexBuffer(s_LineData.LineVertexBuffer);

			RenderCommand::DrawArrays(s_LineData.lineCount);
		}
	}

	void Renderer2D::UploadDataToGpu()
	{
		if (s_LineData.lineCount)
		{
			const uint32_t vertexSize = sizeof(LineVertex) * s_LineData.lineOffset;
			RenderCommand::SetStagingBufferData(s_LineData.LineVerticesBuffer, s_LineData.LineVerticesData.data(),
			                                    vertexSize);
			RenderCommand::CopyStagingBuffer(s_LineData.LineVerticesBuffer, s_LineData.LineVertexBuffer);
		}

		if (s_CircleData.circleCount)
		{
			const uint32_t vertexSize = sizeof(CircleVertex) * s_CircleData.circleOffset;
			RenderCommand::SetStagingBufferData(s_CircleData.CircleVerticesBuffer,
			                                    s_CircleData.CircleVerticesData.data(), vertexSize);
			RenderCommand::CopyStagingBuffer(s_CircleData.CircleVerticesBuffer, s_CircleData.CircleVertexBuffer);
		}

		if (s_QuadData.quadCount)
		{
			uint32_t vertexSize = sizeof(QuadVertex) * s_QuadData.quadOffset;
			//s_QuadData.QuadVertexBuffer->SetData(s_QuadData.QuadVerticesData.data(), vertexSize);
			RenderCommand::SetStagingBufferData(s_QuadData.QuadVerticesBuffer, s_QuadData.QuadVerticesData.data(),
			                                    vertexSize);
			RenderCommand::CopyStagingBuffer(s_QuadData.QuadVerticesBuffer, s_QuadData.QuadVertexBuffer);
		}

		if(s_LineData.lineCount || s_CircleData.circleCount || s_QuadData.quadCount)
		{
			Renderer2DCameraData data{};
			data.view = s_RenderData.m_Renderer2DCamera->GetView();
			data.projection = s_RenderData.m_Renderer2DCamera->GetProjection();

			RenderCommand::SetStagingBufferData(s_RenderData.m_CameraUBData, &data);
			RenderCommand::CopyStagingBuffer(s_RenderData.m_CameraUBData, s_RenderData.m_CameraUB);
		}
	


		Reset();
	}

	GraphicsShaders Renderer2D::LoadShader(const std::string& shaderName)
	{
		const std::string shaderPath = "Renderer2D/";
		const auto shaderLoadVert = shaderPath + shaderName + ".vert";
		const auto shaderLoadFrag = shaderPath + shaderName + ".frag";

		auto vertShader = ShaderRegistry::Load(shaderLoadVert);
		auto fragShader = ShaderRegistry::Load(shaderLoadFrag);

		return {vertShader, fragShader};
	}


	void Renderer2D::Init(const Ref<RenderPass>& renderPass)
	{
		s_RenderData.m_RenderPass = renderPass;
		s_RenderData.m_CameraUB = UniformBufferSet::Create(sizeof(Renderer2DCameraData));
		s_RenderData.m_CameraUBData = StagingBufferSet::Create(sizeof(Renderer2DCameraData));

		PrepareQuads();
		PrepareCircles();
		PrepareLines();


		//temp solution
	}

	void Renderer2D::BeginDraw(const Ref<Camera>& camera)
	{
		s_RenderData.m_Renderer2DCamera = camera;
		//s_RenderData.m_CameraUB->Bind(0);
	}

	void Renderer2D::DebugWindow()
	{
	}

	void Renderer2D::EndDraw()
	{
		PrepareQuadsForRendering();
		PrepareCircleForRendering();
		PrepareLineForRendering();
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
	}

	void Renderer2D::Reset()
	{
		ResetQuads();
		ResetCircles();
		ResetLines();
	}

	void Renderer2D::Shutdown()
	{
		delete s_RenderData.m_CameraData;
		if (!s_QuadData.QuadVerticesData.empty())
		{
			s_QuadData.QuadVerticesData.clear();
		}
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
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
		s_LineData.LineVerticesData.at(s_LineData.lineOffset).Position = p0;
		s_LineData.LineVerticesData.at(s_LineData.lineOffset).color = color;
		s_LineData.LineVerticesData.at(s_LineData.lineOffset + 1).Position = p1;
		s_LineData.LineVerticesData.at(s_LineData.lineOffset + 1).color = color;

		if (s_LineData.lineOffset < s_LineData.vtxSize)
		{
			s_LineData.lineCount += 2;
			s_LineData.lineOffset += 2;
		}
	}

	void Renderer2D::DrawRect(const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color, float thickness)
	{
		glm::vec3 p0 = glm::vec3(pos.x - size.x * 0.5f, pos.y - size.y * 0.5f, pos.z);
		glm::vec3 p1 = glm::vec3(pos.x + size.x * 0.5f, pos.y - size.y * 0.5f, pos.z);
		glm::vec3 p2 = glm::vec3(pos.x + size.x * 0.5f, pos.y + size.y * 0.5f, pos.z);
		glm::vec3 p3 = glm::vec3(pos.x - size.x * 0.5f, pos.y + size.y * 0.5f, pos.z);

		DrawLine(p0, p1, color);
		DrawLine(p1, p2, color);
		DrawLine(p2, p3, color);
		DrawLine(p3, p0, color);
	}

	void Renderer2D::DrawRect(const glm::mat4& transform, const glm::vec4& color)
	{
		glm::vec3 lineVertices[4];
		for (size_t i = 0; i < 4; i++)
			lineVertices[i] = transform * glm::vec4(s_LineData.lineVerts[i].Position, 1.0);


		DrawLine(lineVertices[0], lineVertices[1], color);
		DrawLine(lineVertices[1], lineVertices[2], color);
		DrawLine(lineVertices[2], lineVertices[3], color);
		DrawLine(lineVertices[3], lineVertices[0], color);
	}


	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec4& color)
	{
		;
		glm::vec3 positions[16];

		positions[0] = {1.0f, 1.0f, -1.0f};
		positions[1] = {1.0f, -1.0f, -1.0f};
		positions[2] = {-1.0f, -1.0f, -1.0f};
		positions[3] = {-1.0f, 1.0f, -1.0f};

		positions[4] = {1.0f, 1.0f, 1.0f};
		positions[5] = {1.0f, -1.0f, 1.0f};
		positions[6] = {-1.0f, -1.0f, 1.0f};
		positions[7] = {-1.0f, 1.0f, 1.0f};

		positions[8] = {-1.0f, -1.0f, -1.0f};
		positions[9] = {-1.0f, -1.0f, 1.0f};
		positions[10] = {1.0f, -1.0f, -1.0f};
		positions[11] = {1.0f, -1.0f, 1.0f};

		positions[12] = {-1.0f, 1.0f, -1.0f};
		positions[13] = {-1.0f, 1.0f, 1.0f};
		positions[14] = {1.0f, 1.0f, -1.0f};
		positions[15] = {1.0f, 1.0f, 1.0f};


		for (auto& position : positions)
			position = transform * glm::vec4(position, 1.0);


		DrawLine(positions[0], positions[1], color);
		DrawLine(positions[1], positions[2], color);
		DrawLine(positions[2], positions[3], color);
		DrawLine(positions[3], positions[0], color);

		DrawLine(positions[4], positions[5], color);
		DrawLine(positions[5], positions[6], color);
		DrawLine(positions[6], positions[7], color);
		DrawLine(positions[7], positions[4], color);

		DrawLine(positions[8], positions[9], color);
		DrawLine(positions[10], positions[11], color);
		DrawLine(positions[12], positions[13], color);
		DrawLine(positions[14], positions[15], color);
	}

	void Renderer2D::DrawCube(const glm::mat4& transform, const glm::vec3& extends, const glm::vec4& color)
	{
		glm::vec3 positions[16];

		//-Z
		positions[0] = {1.0f, 1.0f, -1.0f};
		positions[1] = {1.0f, -1.0f, -1.0f};
		positions[2] = {-1.0f, -1.0f, -1.0f};
		positions[3] = {-1.0f, 1.0f, -1.0f};

		//+Z
		positions[4] = {1.0f, 1.0f, 1.0f};
		positions[5] = {1.0f, -1.0f, 1.0f};
		positions[6] = {-1.0f, -1.0f, 1.0f};
		positions[7] = {-1.0f, 1.0f, 1.0f};

		//-Y
		positions[8] = {-1.0f, -1.0f, -1.0f};
		positions[9] = {-1.0f, -1.0f, 1.0f};
		positions[10] = {1.0f, -1.0f, -1.0f};
		positions[11] = {1.0f, -1.0f, 1.0f};

		//+Y
		positions[12] = {-1.0f, 1.0f, -1.0f};
		positions[13] = {-1.0f, 1.0f, 1.0f};
		positions[14] = {1.0f, 1.0f, -1.0f};
		positions[15] = {1.0f, 1.0f, 1.0f};


		positions[0] *= extends;
		positions[1] *= extends;
		positions[2] *= extends;
		positions[3] *= extends;


		positions[4] *= extends;
		positions[5] *= extends;
		positions[6] *= extends;
		positions[7] *= extends;


		positions[8] *= extends;
		positions[9] *= extends;
		positions[10] *= extends;
		positions[11] *= extends;


		positions[12] *= extends;
		positions[13] *= extends;
		positions[14] *= extends;
		positions[15] *= extends;


		for (size_t i = 0; i < 16; i++)
			positions[i] = transform * glm::vec4(positions[i], 1.0);


		//-Z
		DrawLine(positions[0], positions[1], color);
		DrawLine(positions[1], positions[2], color);
		DrawLine(positions[2], positions[3], color);
		DrawLine(positions[3], positions[0], color);

		DrawLine(positions[4], positions[5], color);
		DrawLine(positions[5], positions[6], color);
		DrawLine(positions[6], positions[7], color);
		DrawLine(positions[7], positions[4], color);

		//+Z
		DrawLine(positions[8], positions[9], color);
		DrawLine(positions[10], positions[11], color);
		DrawLine(positions[12], positions[13], color);
		DrawLine(positions[14], positions[15], color);
	}

	void Renderer2D::DrawCube(const glm::mat4& transform, const AABB& aabb, const glm::vec4& color)
	{
		glm::vec3 positions[16];

		auto max = aabb.GetMax();
		auto min = aabb.GetMin();

		positions[0] = {max.x, max.y, min.z};
		positions[1] = {max.x, min.y, min.z};
		positions[2] = {min.x, min.y, min.z};
		positions[3] = {min.x, max.y, min.z};

		positions[4] = {max.x, max.y, max.z};
		positions[5] = {max.x, min.y, max.z};
		positions[6] = {min.x, min.y, max.z};
		positions[7] = {min.x, max.y, max.z};

		positions[8] = {min.x, min.y, min.z};
		positions[9] = {min.x, min.y, max.z};
		positions[10] = {max.x, min.y, min.z};
		positions[11] = {max.x, min.y, max.z};

		positions[12] = {min.x, max.y, min.z};
		positions[13] = {min.x, max.y, max.z};
		positions[14] = {max.x, max.y, min.z};
		positions[15] = {max.x, max.y, max.z};

		for (size_t i = 0; i < 16; i++)
			positions[i] = transform * glm::vec4(positions[i], 1.0);


		DrawLine(positions[0], positions[1], color);
		DrawLine(positions[1], positions[2], color);
		DrawLine(positions[2], positions[3], color);
		DrawLine(positions[3], positions[0], color);

		DrawLine(positions[4], positions[5], color);
		DrawLine(positions[5], positions[6], color);
		DrawLine(positions[6], positions[7], color);
		DrawLine(positions[7], positions[4], color);

		DrawLine(positions[8], positions[9], color);
		DrawLine(positions[10], positions[11], color);
		DrawLine(positions[12], positions[13], color);
		DrawLine(positions[14], positions[15], color);
	}

	void Renderer2D::DrawPyramid(const glm::mat4& transform, float farPlane, float nearPlane, float distance,
	                             const glm::vec4& color)
	{
		glm::vec3 positions[16];

		// -Z Face
		positions[0] = {1.0f, 1.0f, -1.0f};
		positions[1] = {1.0f, -1.0f, -1.0f};
		positions[2] = {-1.0f, -1.0f, -1.0f};
		positions[3] = {-1.0f, 1.0f, -1.0f};


		positions[0] *= nearPlane;
		positions[1] *= nearPlane;
		positions[2] *= nearPlane;
		positions[3] *= nearPlane;
		positions[0].z = -1.0f;
		positions[1].z = -1.0f;
		positions[2].z = -1.0f;
		positions[3].z = -1.0f;

		positions[4] = {1.0f, 1.0f, 1.0f};
		positions[5] = {1.0f, -1.0f, 1.0f};
		positions[6] = {-1.0f, -1.0f, 1.0f};
		positions[7] = {-1.0f, 1.0f, 1.0f};

		positions[8] = {-1.0f, -1.0f, -1.0f};
		positions[9] = {-1.0f, -1.0f, 1.0f};
		positions[10] = {1.0f, -1.0f, -1.0f};
		positions[11] = {1.0f, -1.0f, 1.0f};

		positions[12] = {-1.0f, 1.0f, -1.0f};
		positions[13] = {-1.0f, 1.0f, 1.0f};
		positions[14] = {1.0f, 1.0f, -1.0f};
		positions[15] = {1.0f, 1.0f, 1.0f};


		positions[8].y *= nearPlane;
		positions[10].y *= nearPlane;
		positions[12].y *= nearPlane;
		positions[14].y *= nearPlane;

		positions[8].x *= nearPlane;
		positions[10].x *= nearPlane;
		positions[12].x *= nearPlane;
		positions[14].x *= nearPlane;


		positions[9].y *= farPlane;
		positions[11].y *= farPlane;
		positions[13].y *= farPlane;
		positions[15].y *= farPlane;

		positions[9].z += distance;
		positions[11].z += distance;
		positions[13].z += distance;
		positions[15].z += distance;

		positions[4].z += distance;
		positions[5].z += distance;
		positions[6].z += distance;
		positions[7].z += distance;


		positions[4].y *= farPlane;
		positions[5].y *= farPlane;
		positions[6].y *= farPlane;
		positions[7].y *= farPlane;

		positions[9].x *= farPlane;
		positions[11].x *= farPlane;
		positions[13].x *= farPlane;
		positions[15].x *= farPlane;

		positions[4].x *= farPlane;
		positions[5].x *= farPlane;
		positions[6].x *= farPlane;
		positions[7].x *= farPlane;

		for (size_t i = 0; i < 16; i++)
			positions[i] = (transform * glm::vec4(positions[i], 1.0));

		// -Z Face
		DrawLine(positions[0], positions[1], color);
		DrawLine(positions[1], positions[2], color);
		DrawLine(positions[2], positions[3], color);
		DrawLine(positions[3], positions[0], color);

		DrawLine(positions[4], positions[5], {1.0f, 0.0, 0.0, 1.0f});
		DrawLine(positions[5], positions[6], {1.0f, 0.0, 0.0, 1.0f});
		DrawLine(positions[6], positions[7], {1.0f, 0.0, 0.0, 1.0f});
		DrawLine(positions[7], positions[4], {1.0f, 0.0, 0.0, 1.0f});

		// +Z Face
		DrawLine(positions[8], positions[9], {1.0f, 1.0, 0.0, 1.0});
		DrawLine(positions[10], positions[11], {1.0f, 1.0, 0.0, 1.0});
		DrawLine(positions[12], positions[13], {1.0f, 1.0, 0.0, 1.0});
		DrawLine(positions[14], positions[15], {1.0f, 1.0, 0.0, 1.0});
	}

	void Renderer2D::DrawCameraFrustum(const Ref<Camera>& camera, const glm::vec4& color)
	{
		std::array<glm::vec4, 6> planes = {glm::vec4(1.0f)};

		FrustumCulling::GetFrustumPlanes(camera->GetViewProjection(), planes);

		glm::vec4 leftPlane = planes[0];
		glm::vec4 rightPlane = planes[1];

		glm::vec4 bottomPlane = planes[2];
		glm::vec4 topPlane = planes[3];


		glm::vec4 nearPlane = planes[4];
		glm::vec4 farPlane = planes[5];


		const glm::mat4 invVP = glm::inverse(camera->GetViewProjection());

		glm::vec3 positions[16];

		//-Z
		positions[0] = {rightPlane.x, topPlane.y, farPlane.z};
		positions[1] = {rightPlane.x, bottomPlane.y, farPlane.z};
		positions[2] = {leftPlane.x, bottomPlane.y, farPlane.z};
		positions[3] = {leftPlane.x, topPlane.y, farPlane.z};

		//Z+
		positions[4] = {rightPlane.x, topPlane.y, nearPlane.z};
		positions[5] = {rightPlane.x, bottomPlane.y, nearPlane.z};
		positions[6] = {leftPlane.x, bottomPlane.y, nearPlane.z};
		positions[7] = {leftPlane.x, topPlane.y, nearPlane.z};

		positions[8] = {leftPlane.x, bottomPlane.y, farPlane.z};
		positions[9] = {leftPlane.x, bottomPlane.y, nearPlane.z};
		positions[10] = {rightPlane.x, bottomPlane.y, farPlane.z};
		positions[11] = {rightPlane.x, bottomPlane.y, nearPlane.z};


		positions[12] = {leftPlane.x, topPlane.y, farPlane.z};
		positions[13] = {leftPlane.x, topPlane.y, nearPlane.z};
		positions[14] = {rightPlane.x, topPlane.y, farPlane.z};
		positions[15] = {rightPlane.x, topPlane.y, nearPlane.z};

		for (int i = 0; i < 16; ++i)
		{
			glm::vec4 q = invVP * glm::vec4(positions[i], 1.0f);
			positions[i] = q / q.w;
		}


		DrawLine(positions[0], positions[1], color);
		DrawLine(positions[1], positions[2], color);
		DrawLine(positions[2], positions[3], color);
		DrawLine(positions[3], positions[0], color);

		DrawLine(positions[4], positions[5], color);
		DrawLine(positions[5], positions[6], color);
		DrawLine(positions[6], positions[7], color);
		DrawLine(positions[7], positions[4], color);

		DrawLine(positions[8], positions[9], color);
		DrawLine(positions[10], positions[11], color);
		DrawLine(positions[12], positions[13], color);
		DrawLine(positions[14], positions[15], color);
	}


	void Renderer2D::DrawCircle(const glm::mat4& transform, const glm::vec4& color, float thickness, float fade)
	{
		int count = 0;
		for (auto& circleVert : s_CircleData.circleVerts)
		{
			if (s_CircleData.circleOffset < s_CircleData.vtxSize)
			{
				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).WorldPosition = transform *
					glm::vec4(circleVert.LocalPosition, 1.0f);

				s_CircleData.CircleVerticesData.at(s_CircleData.circleOffset + count).LocalPosition = circleVert.
					LocalPosition;
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
		glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);

		DrawQuad(translate, color);
	}

	void Renderer2D::DrawRotatedQuad(const glm::vec3& position, float rotation, const glm::vec4& color)
	{
		const auto translate = glm::translate(glm::mat4(1.0f), position) * glm::rotate(
			glm::mat4(1.0f), glm::radians(rotation),
			glm::vec3(0, 0, 1));

		DrawQuad(translate, color);
	}
}
