#include "boidpch.h"
#include "VulkanRasterizerState.h"


namespace Polyboid
{
	static vk::PolygonMode ToVulkanPolygonMode(FillMode mode)
	{
		switch (mode)
		{
		case FillMode::Wireframe:
			return vk::PolygonMode::eLine;
		case FillMode::Solid:
			return vk::PolygonMode::eFill;
		case FillMode::Point:
			return vk::PolygonMode::ePoint;
		}
	}

	static vk::FrontFace ToVulkanFrontFace(FrontFaceDirection direction)
	{
		switch (direction)
		{
		case FrontFaceDirection::CounterClockwise: return vk::FrontFace::eCounterClockwise;
		case FrontFaceDirection::Clockwise: return vk::FrontFace::eClockwise;
		}
	}

	VulkanRasterizerState::VulkanRasterizerState()
	{
		Viewport viewport{};
		viewport.Width = 1600;
		viewport.Height = 900;

		m_Viewports.push_back(viewport);

		Rect rect{};
		rect.Width = 1600;
		rect.Height = 900;

		m_ScissorRects.push_back(rect);
	}

	void VulkanRasterizerState::SetFillMode(FillMode frontFace, FillMode backFace)
	{
		m_FrontFace = frontFace;
		m_backFace = backFace;
	}

	void VulkanRasterizerState::GetFillMode(FillMode& frontFace, FillMode& backFace) const
	{
		frontFace = m_FrontFace;
		backFace = m_backFace;
	}

	void VulkanRasterizerState::SetCullMode(CullFaceMode cullMode)
	{
		m_CullMode = cullMode;
	}

	CullFaceMode VulkanRasterizerState::GetCullMode() const
	{
		return m_CullMode;
	}

	void VulkanRasterizerState::SetFrontFacing(FrontFaceDirection frontFace)
	{
		m_FaceDirection = frontFace;
	}

	FrontFaceDirection VulkanRasterizerState::GetFrontFacing() const
	{
		return m_FaceDirection;
	}

	void VulkanRasterizerState::SetDepthBias(const DepthBias& bias)
	{
		m_DepthBias = bias;
	}

	DepthBias VulkanRasterizerState::GetDepthBias() const
	{
		return m_DepthBias;
	}

	void VulkanRasterizerState::SetDepthClipEnabled(bool depthClipEnabled)
	{
		m_DepthClipEnabled = depthClipEnabled;
	}

	bool VulkanRasterizerState::GetDepthClipEnabled() const
	{
		return m_DepthClipEnabled;
	}

	void VulkanRasterizerState::SetScissorEnabled(bool scissorEnable)
	{
		m_ScissorEnabled = scissorEnable;
	}

	bool VulkanRasterizerState::GetScissorEnabled() const
	{
		return m_ScissorEnabled;
	}

	void VulkanRasterizerState::SetScissorRect(const Rect& rect)
	{
		m_ScissorRects[0] = rect;
	}

	void VulkanRasterizerState::SetScissorRects(const std::vector<Rect>& rects)
	{
		m_ScissorRects = rects;
	}

	const std::vector<Rect>& VulkanRasterizerState::GetScissorRects() const
	{
		return m_ScissorRects;
	}

	void VulkanRasterizerState::SetViewport(const Viewport& viewport)
	{
		m_Viewports[0] = viewport;
	}

	void VulkanRasterizerState::SetViewports(const std::vector<Viewport>& viewports)
	{
		m_Viewports = viewports;
	}

	const std::vector<Viewport>& VulkanRasterizerState::GetViewports()
	{
		return m_Viewports;
	}

	void VulkanRasterizerState::SetLineWidth(float size)
	{
		m_LineWidth = size;
	}

	float VulkanRasterizerState::GetLineWidth()
	{
		return m_LineWidth;
	}

	void VulkanRasterizerState::SetPointSize(float size)
	{
		m_PointSize = size;
	}

	float VulkanRasterizerState::GetPointSize()
	{
		return m_PointSize;
	}

	void VulkanRasterizerState::Reset()
	{
		RasterizerState::Reset();
	}


	RasterizerVulkanInfo VulkanRasterizerState::GetVulkanInfo()
	{
		m_DynamicStates = {
			//Require version 1.3
			//vk::DynamicState::eCullMode,
			vk::DynamicState::eViewport,
			vk::DynamicState::eScissor,
			vk::DynamicState::eLineWidth
		};

		m_Info.m_CreateDynamicStateInfo.pDynamicStates = m_DynamicStates.data();
		m_Info.m_CreateDynamicStateInfo.dynamicStateCount = static_cast<uint32_t>(m_DynamicStates.size());

		m_Info.m_CreateRasterizeInfo.depthBiasEnable = false;
		// m_Info.m_CreateRasterizeInfo.depthBiasConstantFactor = m_DepthBias.depthBias;
		// m_Info.m_CreateRasterizeInfo.depthBiasClamp = m_DepthBias.biasClamp;
		m_Info.m_CreateRasterizeInfo.depthClampEnable = false;
		// m_Info.m_CreateRasterizeInfo.depthBiasSlopeFactor = m_DepthBias.slopeBias;
		m_Info.m_CreateRasterizeInfo.lineWidth = m_LineWidth;
		m_Info.m_CreateRasterizeInfo.cullMode = vk::CullModeFlagBits::eNone;
		m_Info.m_CreateRasterizeInfo.polygonMode = ToVulkanPolygonMode(m_FrontFace);
		m_Info.m_CreateRasterizeInfo.frontFace = ToVulkanFrontFace(m_FaceDirection);
		m_Info.m_CreateRasterizeInfo.rasterizerDiscardEnable = false;

		m_Info.m_ViewportCreateInfo.viewportCount = static_cast<uint32_t>(m_Viewports.size());
		m_Info.m_ViewportCreateInfo.scissorCount = static_cast<uint32_t>(m_ScissorRects.size());

		// for (const auto& scissor : m_ScissorRects)
		// {
		// 	vk::Rect2D vkScissor;
		// 	vkScissor.extent.height = static_cast<uint32_t>(scissor.Height);
		// 	vkScissor.extent.width = static_cast<uint32_t>(scissor.Width);
		// 	vkScissor.offset.x = static_cast<int32_t>(scissor.X);
		// 	vkScissor.offset.y = static_cast<int32_t>(scissor.Y);
		//
		// 	m_VkScissorRects.emplace_back(vkScissor);
		// }
		//


		// for (const auto& viewport : m_Viewports)
		// {
		// 	vk::Viewport vkViewport;
		// 	vkViewport.width = viewport.Width;
		// 	vkViewport.height = viewport.Height;
		// 	vkViewport.x = viewport.X;
		// 	vkViewport.y = viewport.Y;
		// 	vkViewport.minDepth = viewport.MinDepth;
		// 	vkViewport.maxDepth = viewport.MaxDepth;
		// 	m_VkViewports.emplace_back(vkViewport);
		// }


		m_Dirty = false;

		return m_Info;
	}
}
