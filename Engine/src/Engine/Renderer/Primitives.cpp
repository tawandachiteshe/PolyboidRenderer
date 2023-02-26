#include "boidpch.h"
#include "Primitives.h"

#include "VertexBufferArray.h"

namespace Polyboid
{
	Ref<VertexBufferArray> Primitives::GenQuad()
	{
		uint32_t indices[6] = { 1, 2, 3, 0, 1, 3 };
		Vertex vtx[4];
		vtx[0] = { {1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f} };
		vtx[1] = { {1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f} };
		vtx[2] = { {-1.0f, -1.0f, 0.0f,}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f} };
		vtx[3] = { {-1.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} };
		//
		// auto indicesBuffer = ShaderBufferStorage::Make(indices, 6 * sizeof(uint32_t));
		//
		// auto va = VertexBufferArray::MakeVertexBufferArray(indicesBuffer, 6);
		//
		// auto verts = ShaderBufferStorage::Make(vtx, sizeof(vtx));
		// va->SetShaderBufferStorage(verts);

		return nullptr;
		
	}						  
}				
