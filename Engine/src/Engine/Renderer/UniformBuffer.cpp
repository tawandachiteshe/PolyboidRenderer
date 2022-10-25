#include "UniformBuffer.h"
#include <glad/glad.h>

namespace Polyboid {



	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
	{
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		glNamedBufferSubData(m_ID, offset, size, data);
	}

	UniformBuffer::~UniformBuffer()
	{
		glDeleteBuffers(1, &m_ID);
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::MakeUniformBuffer(uint32_t size, uint32_t binding)
	{
		return std::make_shared<UniformBuffer>(size, binding);
	}

}