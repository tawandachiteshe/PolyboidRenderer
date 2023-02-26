#pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{

#define   ALLOC_COMMAND(Klass, ...) std::make_shared<Klass>(__VA_ARGS__)

	class RenderTarget;
	class PipelineState;
	class Command;
	class RenderAPI;


	struct RenderCommandData
	{
		Ref<RenderAPI> m_Context;
		std::vector<Ref<Command>> m_Commands;
	};


	class RenderCommand
	{
	private:
		static Unique<RenderCommandData> s_Data;
	public:
		static void Init(const Ref<RenderAPI>& context);
		static void AddCommand(const Ref<Command>& renderCommand);
		static void WaitAndRender();
	};

}
