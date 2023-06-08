#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class CommandBuffer;
	class CommandBufferSet;

	struct KomputeRenderData
	{

		Ref<CommandBufferSet> m_CurrentCommand;
		Ref<CommandBuffer> m_CommandBuffer;
		uint32_t m_CurrentIndex;
	};

	class KomputeCommand
	{

	private:

		static ScopePtr<KomputeRenderData> s_Data;

	public:

		static void Init();

		static void BeginCommands(const Ref<CommandBufferSet>& m_CurrentCommand, uint32_t index = 0);
		static void EndCommands();

		static Ref<CommandBuffer> GetCurrentCommandBuffer();
		static void SetCurrentCommandBuffer(uint32_t currentIndex);


	};

}

