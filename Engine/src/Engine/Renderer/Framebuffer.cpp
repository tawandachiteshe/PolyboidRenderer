#include "boidpch.h"
#include "Framebuffer.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<Framebuffer> Framebuffer::Create(const Ref<RenderPass>& renderPass)
	{
		return RenderAPI::Get()->CreateFrameBuffer(renderPass);
	}
}
