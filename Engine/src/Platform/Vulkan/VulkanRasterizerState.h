﻿#pragma once
#include "Engine/Renderer/RasterizerState.h"
#include <vulkan/vulkan.hpp>

namespace Polyboid
{
	struct RasterizerVulkanInfo
	{
		vk::PipelineDynamicStateCreateInfo m_CreateDynamicStateInfo;
		vk::PipelineRasterizationStateCreateInfo m_CreateRasterizeInfo;
		vk::PipelineViewportStateCreateInfo m_ViewportCreateInfo;

	};

	class VulkanRasterizerState : public RasterizerState
	{
	private:

		RasterizerVulkanInfo m_Info{};

		FillMode m_FrontFace = FillMode::Solid;
		FillMode m_backFace = FillMode::Solid;
		bool m_ScissorEnabled = false;
		bool m_DepthClipEnabled = false;
		std::vector<Viewport> m_Viewports;
		std::vector<vk::Viewport> m_VkViewports;
		std::vector<vk::Rect2D> m_VkScissorRects;
		std::vector<Rect> m_ScissorRects;
		FrontFaceDirection m_FaceDirection = FrontFaceDirection::Clockwise;
		CullFaceMode m_CullMode = CullFaceMode::Back;
		DepthBias m_DepthBias;
		std::vector<vk::DynamicState> m_DynamicStates;

		float m_LineWidth = 1.0f;
		float m_PointSize = 1.0f;

		bool m_Dirty = true;

	public:

		VulkanRasterizerState();

		void SetFillMode(FillMode frontFace, FillMode backFace) override;
		void GetFillMode(FillMode& frontFace, FillMode& backFace) const override;
		void SetCullMode(CullFaceMode cullMode) override;
		CullFaceMode GetCullMode() const override;
		void SetFrontFacing(FrontFaceDirection frontFace) override;
		FrontFaceDirection GetFrontFacing() const override;
		void SetDepthBias(const DepthBias& bias) override;
		DepthBias GetDepthBias() const override;
		void SetDepthClipEnabled(bool depthClipEnabled) override;
		bool GetDepthClipEnabled() const override;
		void SetScissorEnabled(bool scissorEnable) override;
		bool GetScissorEnabled() const override;
		void SetScissorRect(const Rect& rect) override;
		void SetScissorRects(const std::vector<Rect>& rects) override;
		const std::vector<Rect>& GetScissorRects() const override;
		void SetViewport(const Viewport& viewport) override;
		void SetViewports(const std::vector<Viewport>& viewports) override;
		const std::vector<Viewport>& GetViewports() override;
		void SetLineWidth(float size) override;
		float GetLineWidth() override;
		void SetPointSize(float size) override;
		float GetPointSize() override;
		void Reset() override;

		virtual RasterizerVulkanInfo GetVulkanInfo();

		~VulkanRasterizerState() override = default;
	};

}

