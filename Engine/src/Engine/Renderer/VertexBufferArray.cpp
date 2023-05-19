
#include "boidpch.h"
#include "VertexBufferArray.h"



namespace Polyboid
{
	Ref<VertexBufferArray> VertexBufferArray::Create()
	{
		return RenderAPI::Get()->CreateVertexBufferArray();
	}
}
