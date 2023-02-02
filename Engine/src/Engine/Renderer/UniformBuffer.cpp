#include "boidpch.h"
#include "UniformBuffer.h"


#include <glad/glad.h>

#include "Engine/Engine/Debug/Profiler.h"

namespace Polyboid {



	UniformBuffer::UniformBuffer(uint32_t size, uint32_t binding)
	{
		POLYBOID_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_ID);
		glNamedBufferData(m_ID, size, nullptr, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
	}

	void UniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		POLYBOID_PROFILE_FUNCTION();
		glNamedBufferSubData(m_ID, offset, size, data);
	}

	UniformBuffer::~UniformBuffer()
	{
		POLYBOID_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_ID);
	}

	void UniformBuffer::Bind(uint32_t binding)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_ID);
	}

	std::shared_ptr<UniformBuffer> UniformBuffer::MakeUniformBuffer(uint32_t size, uint32_t binding)
	{
		POLYBOID_PROFILE_FUNCTION();
		return std::make_shared<UniformBuffer>(size, binding);
	}

}
