#include "boidpch.h"
#include "VulkanDepthStencilState.h"


namespace Polyboid
{

	vk::CompareOp depthFunctionToVkCompareOp(DepthFunction depthFunction)
	{
		switch (depthFunction)
		{
		case DepthFunction::Never:
			return vk::CompareOp::eNever;
		case DepthFunction::Less:
			return vk::CompareOp::eLess;
		case DepthFunction::Equal:
			return vk::CompareOp::eEqual;
		case DepthFunction::LessThanOrEqual:
			return vk::CompareOp::eLessOrEqual;
		case DepthFunction::Greater:
			return vk::CompareOp::eGreater;
		case DepthFunction::NotEqual:
			return vk::CompareOp::eNotEqual;
		case DepthFunction::GreaterThanOrEqual:
			return vk::CompareOp::eGreaterOrEqual;
		case DepthFunction::Always:
			return vk::CompareOp::eAlways;
		default:
			// Handle invalid DepthFunction value
			throw std::invalid_argument("Invalid DepthFunction value");
		}
	}

	vk::StencilOp stencilOpToVkStencilOp(StencilOp stencilOp)
	{
		switch (stencilOp)
		{
		case StencilOp::Keep:
			return vk::StencilOp::eKeep;
		case StencilOp::Zero:
			return vk::StencilOp::eZero;
		case StencilOp::Replace:
			return vk::StencilOp::eReplace;
		case StencilOp::Increment:
			return vk::StencilOp::eIncrementAndClamp;
		case StencilOp::IncrementWrap:
			return vk::StencilOp::eIncrementAndWrap;
		case StencilOp::Decrement:
			return vk::StencilOp::eDecrementAndClamp;
		case StencilOp::DecrementWrap:
			return vk::StencilOp::eDecrementAndWrap;
		case StencilOp::Invert:
			return vk::StencilOp::eInvert;
		default:
			// Handle invalid StencilOp value
			throw std::invalid_argument("Invalid StencilOp value");
		}
	}

	vk::CompareOp stencilFunctionToVkCompareOp(StencilFunction stencilFunction)
	{
		switch (stencilFunction)
		{
		case StencilFunction::Never:
			return vk::CompareOp::eNever;
		case StencilFunction::Less:
			return vk::CompareOp::eLess;
		case StencilFunction::Equal:
			return vk::CompareOp::eEqual;
		case StencilFunction::LessThanOrEqual:
			return vk::CompareOp::eLessOrEqual;
		case StencilFunction::Greater:
			return vk::CompareOp::eGreater;
		case StencilFunction::NotEqual:
			return vk::CompareOp::eNotEqual;
		case StencilFunction::GreaterThanOrEqual:
			return vk::CompareOp::eGreaterOrEqual;
		case StencilFunction::Always:
			return vk::CompareOp::eAlways;
		default:
			// Handle invalid DepthFunction value
			throw std::invalid_argument("Invalid DepthFunction value");
		}
	}

	void VulkanDepthStencilState::SetDepthMode(const DepthMode& depthMode)
	{
		m_DepthMode = depthMode;
	}

	const DepthMode& VulkanDepthStencilState::GetDepthMode() const
	{
		return  m_DepthMode;
	}

	void VulkanDepthStencilState::SetStencilMode(const StencilMode& stencilMode)
	{
		m_StencilMode = stencilMode;
	}

	const StencilMode& VulkanDepthStencilState::GetStencilMode() const
	{
		return m_StencilMode;
	}

	VulkanDepthStencilState::~VulkanDepthStencilState()
	{
	}

	void VulkanDepthStencilState::Bind()
	{
		if (m_Dirty)
		{

			m_CreateInfo.minDepthBounds = 0.0f;
			m_CreateInfo.maxDepthBounds = 1.0f;
			m_CreateInfo.depthBoundsTestEnable = m_DepthMode.DepthEnable;
			m_CreateInfo.depthWriteEnable = m_DepthMode.DepthWriteMask == DepthWrite::Enable;
			m_CreateInfo.depthCompareOp = depthFunctionToVkCompareOp(m_DepthMode.depthFunction);

			m_CreateInfo.stencilTestEnable = m_StencilMode.StencilEnabled;

			vk::StencilOpState front;
			auto frontFace = m_StencilMode.FrontFace;
			front.compareOp = stencilFunctionToVkCompareOp(frontFace.stencilFunction);
			front.depthFailOp = stencilOpToVkStencilOp(frontFace.StencilPassDepthFail);
			front.failOp = stencilOpToVkStencilOp(frontFace.StencilFail);
			front.passOp = stencilOpToVkStencilOp(frontFace.StencilDepthPass);
			front.reference = m_StencilMode.StencilReference;
			front.compareMask = 0xFF;

			m_CreateInfo.front = front;

			vk::StencilOpState back;
			auto backFace = m_StencilMode.BackFace;
			back.compareOp = stencilFunctionToVkCompareOp(backFace.stencilFunction);
			back.depthFailOp = stencilOpToVkStencilOp(backFace.StencilPassDepthFail);
			back.failOp = stencilOpToVkStencilOp(backFace.StencilFail);
			back.passOp = stencilOpToVkStencilOp(backFace.StencilDepthPass);
			back.reference = m_StencilMode.StencilReference;
			back.compareMask = 0xFF;

			m_CreateInfo.back = back;

			m_Dirty = false;
		}

	}
}
