#include "boidpch.h"
#include "ShaderBufferStorage.h"

#include "glad/glad.h"

namespace Polyboid
{
	ShaderBufferStorage::ShaderBufferStorage(const void* data, uint32_t dataSize): m_ID(0)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, dataSize, data, 0);
	}

	ShaderBufferStorage::ShaderBufferStorage(uint32_t dataSize): m_ID(0)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferStorage(m_ID, dataSize, nullptr, GL_DYNAMIC_STORAGE_BIT);
	}

	void ShaderBufferStorage::SetData(const void* data, int32_t dataSize, int32_t dataOffset)
	{
		glNamedBufferSubData(m_ID, dataOffset, dataSize, data);
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

	Ref<ShaderBufferStorage> ShaderBufferStorage::Make(uint32_t dataSize)
	{
		return std::make_shared<ShaderBufferStorage>(dataSize);
	}
}
