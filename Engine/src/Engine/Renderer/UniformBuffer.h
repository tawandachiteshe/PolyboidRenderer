#pragma once
#include <cstdint>
#include <memory>


namespace Polyboid {
	class UniformBuffer
	{
	private:
		uint32_t m_ID;
	public:
		UniformBuffer(uint32_t size, uint32_t binding);
		void SetData(const void* data, uint32_t size, uint32_t offset = 0);
		~UniformBuffer();

		static std::shared_ptr<UniformBuffer> MakeUniformBuffer(uint32_t size, uint32_t binding = 0);
	};
}


