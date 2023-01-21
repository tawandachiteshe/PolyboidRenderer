#include "boidpch.h"
#include "ShaderBufferStorage.h"

#include "glad/glad.h"

namespace Polyboid
{
	ShaderBufferStorage::ShaderBufferStorage(const void* data, uint32_t dataSize)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, dataSize, data, 0);
	}

	void ShaderBufferStorage::Bind(uint32_t bindSlot)
	{
		glBindBufferBase(
			GL_SHADER_STORAGE_BUFFER, bindSlot, m_ID);
	}

	Ref<ShaderBufferStorage> ShaderBufferStorage::Make(const void* data, uint32_t dataSize)
	{
		return std::make_shared<ShaderBufferStorage>(data, dataSize);
	}
}
