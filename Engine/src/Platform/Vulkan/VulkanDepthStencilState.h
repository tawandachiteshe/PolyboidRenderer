#pragma once
#include "Engine/Renderer/DepthStencilState.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	class VulkanDepthStencilState : public DepthStencilState
	{

	private:
		DepthMode m_DepthMode;
		StencilMode m_StencilMode;
		bool m_Dirty = true;
		vk::PipelineDepthStencilStateCreateInfo m_CreateInfo;

	public:

		VulkanDepthStencilState() = default;

		void SetDepthMode(const DepthMode& depthMode) override;
		const DepthMode& GetDepthMode() const override;
		void SetStencilMode(const StencilMode& stencilMode) override;
		const StencilMode& GetStencilMode() const override;
		~VulkanDepthStencilState() override;

		virtual void Bind();

	};

}

