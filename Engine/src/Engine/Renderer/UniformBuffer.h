#pragma once
#include <cstdint>


namespace Polyboid {
	
	class UniformBuffer
	{
	public:
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0;
		virtual  ~UniformBuffer() = default;

		//Debugging stuff idk.... Im testing new systems
		virtual uint32_t GetBindingSlot() = 0;
		virtual uint32_t GetDataSize() = 0;
	}

	;
}


