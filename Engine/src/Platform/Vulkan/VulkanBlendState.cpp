#include "boidpch.h"
#include "VulkanBlendState.h"
#include <vulkan/vulkan.hpp>

#include "VkRenderAPI.h"
#include "Utils/VulkanDevice.h"

namespace Polyboid
{
	static vk::BlendFactor ToVkBlendFactor(BlendFactor blend_factor)
	{
		switch (blend_factor)
		{
		case BlendFactor::Zero:
			return vk::BlendFactor::eZero;
		case BlendFactor::One:
			return vk::BlendFactor::eOne;
		case BlendFactor::SrcColor:
			return vk::BlendFactor::eSrcColor;
		case BlendFactor::OneMinusSrcColor:
			return vk::BlendFactor::eOneMinusSrcColor;
		case BlendFactor::DstColor:
			return vk::BlendFactor::eDstColor;
		case BlendFactor::OneMinusDstColor:
			return vk::BlendFactor::eOneMinusDstColor;
		case BlendFactor::SrcAlpha:
			return vk::BlendFactor::eSrcAlpha;
		case BlendFactor::OneMinusSrcAlpha:
			return vk::BlendFactor::eOneMinusSrcAlpha;
		case BlendFactor::DstAlpha:
			return vk::BlendFactor::eDstAlpha;
		case BlendFactor::OneMinusDstAlpha:
			return vk::BlendFactor::eOneMinusDstAlpha;
		case BlendFactor::ConstantColor:
			return vk::BlendFactor::eConstantColor;
		case BlendFactor::OneMinusConstantColor:
			return vk::BlendFactor::eOneMinusConstantColor;
		case BlendFactor::ConstantAlpha:
			return vk::BlendFactor::eConstantAlpha;
		case BlendFactor::OneMinusConstantAlpha:
			return vk::BlendFactor::eOneMinusConstantAlpha;
		case BlendFactor::SrcAlphaSaturate:
			return vk::BlendFactor::eSrcAlphaSaturate;
		case BlendFactor::Src1Color:
			return vk::BlendFactor::eSrc1Color;
		case BlendFactor::OneMinusSrc1Color:
			return vk::BlendFactor::eOneMinusSrc1Color;
		case BlendFactor::Src1Alpha:
			return vk::BlendFactor::eSrc1Alpha;
		case BlendFactor::OneMinusSrc1Alpha:
			return vk::BlendFactor::eOneMinusSrc1Alpha;
		default:
			throw std::invalid_argument("Invalid blend factor.");
		}
	}

	static vk::LogicOp ToVulkanLogicOP(BlendLogicOperation blend_logic_op)
	{
		switch (blend_logic_op)
		{
		case BlendLogicOperation::Clear:
			return vk::LogicOp::eClear;
		case BlendLogicOperation::Set:
			return vk::LogicOp::eSet;
		case BlendLogicOperation::Copy:
			return vk::LogicOp::eCopy;
		case BlendLogicOperation::CopyInverted:
			return vk::LogicOp::eCopyInverted;
		case BlendLogicOperation::Noop:
			return vk::LogicOp::eNoOp;
		case BlendLogicOperation::Invert:
			return vk::LogicOp::eInvert;
		case BlendLogicOperation::And:
			return vk::LogicOp::eAnd;
		case BlendLogicOperation::Nand:
			return vk::LogicOp::eNand;
		case BlendLogicOperation::Or:
			return vk::LogicOp::eOr;
		case BlendLogicOperation::Nor:
			return vk::LogicOp::eNor;
		case BlendLogicOperation::Xor:
			return vk::LogicOp::eXor;
		case BlendLogicOperation::Equiv:
			return vk::LogicOp::eEquivalent;
		case BlendLogicOperation::AndReverse:
			return vk::LogicOp::eAndReverse;
		case BlendLogicOperation::AndInverted:
			return vk::LogicOp::eAndInverted;
		case BlendLogicOperation::OrReverse:
			return vk::LogicOp::eOrReverse;
		case BlendLogicOperation::OrInverted:
			return vk::LogicOp::eOrInverted;
		}
	}

	vk::BlendOp ToVKBlendOperation(BlendOperation blend_op)
	{
		switch (blend_op)
		{
		case BlendOperation::Add:
			return vk::BlendOp::eAdd;
		case BlendOperation::Subtract:
			return vk::BlendOp::eSubtract;
		case BlendOperation::ReverseSubtract:
			return vk::BlendOp::eReverseSubtract;
		case BlendOperation::Min:
			return vk::BlendOp::eMin;
		case BlendOperation::Max:
			return vk::BlendOp::eMax;
		default:
			throw std::invalid_argument("Invalid blend operation.");
		}
	}

	VulkanBlendState::VulkanBlendState(): m_ConstBlend(0)
	{
		m_BlendModes.reserve(8);
		BlendMode mode{};
		mode.LogicOpEnabled = false;
		mode.BlendEnabled = false;
		m_BlendModes.push_back(mode);
	}

	void VulkanBlendState::SetBlendMode(const BlendMode& blendMode)
	{
		m_BlendMode = blendMode;
	}

	void VulkanBlendState::SetBlendModes(const std::vector<BlendMode>& blendModes)
	{
		m_BlendModes = blendModes;
	}

	const std::vector<BlendMode>& VulkanBlendState::GetBlendModes() const
	{
		return m_BlendModes;
	}


	void VulkanBlendState::SetIndependentBlend(bool enabled)
	{
	}

	bool VulkanBlendState::GetIndependentBlend() const
	{
		return false;
	}

	void VulkanBlendState::SetConstBlendFactor(const glm::vec4& constantBlendFactor)
	{
		m_ConstBlend = constantBlendFactor;
	}

	const glm::vec4& VulkanBlendState::GetConstBlendFactor() const
	{
		return m_ConstBlend;
	}

	VulkanBlendState::~VulkanBlendState()
	{
	}


	vk::PipelineColorBlendStateCreateInfo VulkanBlendState::GetVulkanInfo()
	{
		if (m_Dirty)
		{
			m_CreateInfo.flags = vk::PipelineColorBlendStateCreateFlags();
			m_CreateInfo.blendConstants[0] = m_ConstBlend[0];
			m_CreateInfo.blendConstants[1] = m_ConstBlend[1];
			m_CreateInfo.blendConstants[2] = m_ConstBlend[2];
			m_CreateInfo.blendConstants[3] = m_ConstBlend[3];
			m_CreateInfo.logicOpEnable = m_BlendMode.LogicOpEnabled;
			m_CreateInfo.logicOp = ToVulkanLogicOP(m_BlendMode.LogicOp);


			for (auto& blendMode : m_BlendModes)
			{
				vk::PipelineColorBlendAttachmentState attachmentState;
				attachmentState.blendEnable = blendMode.BlendEnabled;
				attachmentState.alphaBlendOp = ToVKBlendOperation(blendMode.AlphaOp);
				attachmentState.colorBlendOp = ToVKBlendOperation(blendMode.BlendOp);
				attachmentState.colorWriteMask = vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
					vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
				attachmentState.dstAlphaBlendFactor = ToVkBlendFactor(blendMode.DstAlphaFactor);
				attachmentState.srcAlphaBlendFactor = ToVkBlendFactor(blendMode.SrcAlphaFactor);
				attachmentState.dstColorBlendFactor = ToVkBlendFactor(blendMode.DstFactor);
				attachmentState.srcColorBlendFactor = ToVkBlendFactor(blendMode.SrcFactor);
				m_AttachmentStates.push_back(attachmentState);
			}

			m_CreateInfo.pAttachments = m_AttachmentStates.data();
			m_CreateInfo.attachmentCount = static_cast<uint32_t>(m_AttachmentStates.size());
			m_Dirty = false;
		}
	



		return m_CreateInfo;
	}
}
