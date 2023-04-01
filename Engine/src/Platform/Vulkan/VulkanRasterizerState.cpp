#include "boidpch.h"
#include "VulkanRasterizerState.h"


namespace Polyboid
{

	static vk::PolygonMode ToVulkanPolygonMode(FillMode mode) {
		switch (mode) {
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
		m_Viewports.resize(8, Viewport());
		m_ScissorRects.resize(8, Rect());
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
		return  m_FaceDirection;
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

	void VulkanRasterizerState::Bind()
	{
		if (m_Dirty)
		{
			std::vector<vk::DynamicState> dynamicState = {
				//Require version 1.3
				//vk::DynamicState::eCullMode,
				vk::DynamicState::eViewport,
				vk::DynamicState::eScissor,
			};

			m_CreateStateInfo.pDynamicStates = dynamicState.data();
			m_CreateStateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicState.size());

			m_CreateInfo.depthBiasEnable = m_DepthBias.depthBias > 0.0f;
			m_CreateInfo.depthBiasConstantFactor = m_DepthBias.depthBias;
			m_CreateInfo.depthBiasClamp = m_DepthBias.biasClamp;
			m_CreateInfo.depthClampEnable = m_DepthBias.biasClamp > 0.0f;
			m_CreateInfo.depthBiasSlopeFactor = m_DepthBias.slopeBias;
			m_CreateInfo.lineWidth = m_LineWidth;
			m_CreateInfo.polygonMode = ToVulkanPolygonMode(m_FrontFace);
			m_CreateInfo.frontFace = ToVulkanFrontFace(m_FaceDirection);

		

			std::vector<vk::Viewport> vkViewports;

			for (const auto& viewport : m_Viewports)
			{
				vk::Viewport vkViewport;
				vkViewport.width = viewport.Width;
				vkViewport.height = viewport.Height;
				vkViewport.x = viewport.X;
				vkViewport.y = viewport.Y;
				vkViewport.minDepth = viewport.MinDepth;
				vkViewport.maxDepth = viewport.MaxDepth;
				vkViewports.emplace_back(vkViewport);
			}

			m_ViewportCreateInfo.viewportCount = static_cast<uint32_t>(m_Viewports.size());
			m_ViewportCreateInfo.pViewports = vkViewports.data();

			std::vector<vk::Rect2D> vkScissors;
			for (const auto& scissor : m_ScissorRects)
			{
				vk::Rect2D vkScissor;
				vkScissor.extent.height = scissor.Height;
				vkScissor.extent.width = scissor.Width;
				vkScissor.offset.x = scissor.X;
				vkScissor.offset.y = scissor.Y;

				vkScissors.emplace_back(vkScissor);
			}


			m_ViewportCreateInfo.pScissors = vkScissors.data();
			m_ViewportCreateInfo.scissorCount = static_cast<uint32_t>(m_ScissorRects.size());

			m_Dirty = false;
		}
	}
}
