          #pragma once
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class RenderAPI;
	class CommandBufferSet;

	class ApplicationRenderEngine
	{

	private:
		
		Scope<RenderAPI> m_RenderAPI = nullptr;

	public:

		ApplicationRenderEngine(const Scope<RenderAPI>& renderAPI);

	};

}

