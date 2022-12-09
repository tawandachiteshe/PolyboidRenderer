#pragma once
#include "RenderAPI.h"


namespace Polyboid
{
	class RenderCommand
	{
	public:

		static void DrawIndexed(uint32_t count, uint16_t elementCount = 4)
		{
			RenderAPI::DrawIndexed(count, elementCount);
		}

	};

}
