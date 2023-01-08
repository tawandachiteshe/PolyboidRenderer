#include "boidpch.h"
#include "Primitives.h"

#include "VertexBufferArray.h"

namespace Polyboid
{
	Ref<VertexBufferArray> Primitives::GenQuad()
	{
		auto vtxArray = VertexBufferArray::MakeVertexBufferArray();
		vtxArray->Bind();

		uint32_t indices[6] = { 1, 2, 3, 0, 1, 3 };
		auto idxBuffer = IndexBuffer::MakeIndexBuffer(indices, 6);

		Vertex vtx[4];
		vtx[0] = { {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		vtx[1] = { {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };
		vtx[2] = { {-1.0f, -1.0f, 0.0f,}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		vtx[3] = { {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };

		auto vtxBuffer = VertexBuffer::MakeVertexBuffer(vtx, sizeof(vtx));
		vtxBuffer->DescribeBuffer({
			{ "aPosition", BufferComponent::Float3 },
			{ "aNormal", BufferComponent::Float3 },
			{ "aUV", BufferComponent::Float2 }
		});
		vtxBuffer->SetIndexBuffer(idxBuffer);
		vtxArray->SetIndexBuffer(idxBuffer);

		vtxArray->AddVertexBuffer(vtxBuffer);

		return vtxArray;
		
	}						  
}				
