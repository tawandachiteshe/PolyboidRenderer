#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class ShaderBufferStorage
	{
	private:
		uint32_t m_ID;
	public:
		ShaderBufferStorage(const void* data, uint32_t dataSize);
		uint32_t GetID() { return m_ID; }

		//Tenmp

		void Bind(uint32_t bindSlot = 0);

		static Ref<ShaderBufferStorage> Make(const void* data, uint32_t dataSize);
	};
}
