#include "boidpch.h"
#include "VulkanTexelBuffers.h"

#include "VkRenderAPI.h"
#include "Utils/Common.h"


namespace Polyboid
{
	VulkanTexelStorageBuffer::VulkanTexelStorageBuffer(const TexelBufferSettings& settings) : m_Settings(settings), m_Allocator("Texel Storage buffer")
	{
		Init(settings);
	}

	vk::BufferView VulkanTexelStorageBuffer::GetBufferView()
	{
		return m_BufferView;
	}


	void VulkanTexelStorageBuffer::Init(const TexelBufferSettings& settings)
	{
		const auto device = VkRenderAPI::GetVulkanDevice();
		m_Size = Utils::CalculateVulkanFormatToBytes(settings.format) * settings.width * settings.height;


		const auto& [buffer, _] = m_Allocator.CreateBuffer(m_Size, vk::BufferUsageFlagBits::eStorageTexelBuffer);
		m_Buffer = buffer;


		vk::BufferViewCreateInfo createInfo{};
		createInfo.buffer = buffer;
		createInfo.format = Utils::ConvertToVulkanFormat(settings.format);
		createInfo.range = m_Size;
		createInfo.offset = 0;


		const auto [result, bufferView] = device.createBufferView(createInfo);
		vk::resultCheck(result, "Failed to create buffer view");


		m_BufferView = bufferView;
		m_BufferDescInfo.buffer = m_Buffer;
		m_BufferDescInfo.offset = 0;
		m_BufferDescInfo.range = m_Size;
	}

	std::any VulkanTexelStorageBuffer::GetHandle()
	{
		return m_Buffer;
	}

	EngineGraphicsFormats VulkanTexelStorageBuffer::GetImageFormat()
	{
		return m_Settings.format;
	}

	ImageLayout VulkanTexelStorageBuffer::GetLayout()
	{
		return ImageLayout::Undefined;
	}

	uint32_t VulkanTexelStorageBuffer::GetHeight()
	{
		return m_Settings.height;
	}

	uint32_t VulkanTexelStorageBuffer::GetWidth()
	{
		return m_Settings.width;
	}

	VulkanTexelStorageBuffer::~VulkanTexelStorageBuffer()
	{
	}

	vk::DescriptorBufferInfo VulkanTexelStorageBuffer::GetBufferDescInfo()
	{
		return m_BufferDescInfo;
	}

	void VulkanTexelStorageBuffer::Destroy()
	{
		const auto device = VkRenderAPI::GetVulkanDevice();
		device.destroyBufferView(m_BufferView);
		m_Allocator.DestroyBuffer();
	}

	void VulkanTexelStorageBuffer::Recreate()
	{
		Destroy();
		Init(m_Settings);
	}

	RenderResourceType VulkanTexelStorageBuffer::GetRenderResourceType()
	{
		return RenderResourceType::TexelStorageBuffer;
	}


	///////////////////////////////////////////////////////// UNIFORM BUFFER: ////////////////////////////////////////////////////////////////


	void VulkanTexelUniformBuffer::Init(const TexelBufferSettings& settings)
	{
		const auto device = VkRenderAPI::GetVulkanDevice();
		m_Size = Utils::CalculateVulkanFormatToBytes(settings.format) * settings.width * settings.height;


		const auto& [buffer, _] = m_Allocator.CreateBuffer(m_Size, vk::BufferUsageFlagBits::eUniformTexelBuffer);
		m_Buffer = buffer;


		vk::BufferViewCreateInfo createInfo{};
		createInfo.buffer = m_Buffer;
		createInfo.format = Utils::ConvertToVulkanFormat(settings.format);
		createInfo.range = m_Size;
		createInfo.offset = 0;

		const auto [result, bufferView] = device.createBufferView(createInfo);
		vk::resultCheck(result, "Failed to create buffer view");

		m_BufferView = bufferView;

		m_BufferDescInfo.buffer = buffer;
		m_BufferDescInfo.offset = 0;
		m_BufferDescInfo.range = m_Size;

	}

	VulkanTexelUniformBuffer::VulkanTexelUniformBuffer(const TexelBufferSettings& settings): m_Allocator("Uniform buffer texel"), m_Settings(settings)
	{
		Init(settings);
	}

	vk::DescriptorBufferInfo VulkanTexelUniformBuffer::GetBufferDescInfo()
	{
		return m_BufferDescInfo;
	}

	vk::BufferView VulkanTexelUniformBuffer::GetBufferView()
	{
		return m_BufferView;
	}


	std::any VulkanTexelUniformBuffer::GetHandle()
	{
		return m_Buffer;
	}

	EngineGraphicsFormats VulkanTexelUniformBuffer::GetImageFormat()
	{
		return m_Settings.format;
	}

	ImageLayout VulkanTexelUniformBuffer::GetLayout()
	{
		return ImageLayout::Undefined;
	}

	uint32_t VulkanTexelUniformBuffer::GetHeight()
	{
		return m_Settings.height;
	}

	uint32_t VulkanTexelUniformBuffer::GetWidth()
	{
		return m_Settings.width;
	}

	void VulkanTexelUniformBuffer::Destroy()
	{
		const auto device = VkRenderAPI::GetVulkanDevice();
		device.destroyBufferView(m_BufferView);
		m_Allocator.DestroyBuffer();
	}

	void VulkanTexelUniformBuffer::Recreate()
	{
		Destroy();
		Init(m_Settings);
	}

	RenderResourceType VulkanTexelUniformBuffer::GetRenderResourceType()
	{
		return RenderResourceType::TexelUniformBuffer;
	}
}
