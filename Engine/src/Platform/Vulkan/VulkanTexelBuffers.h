#pragma once
#include "Engine/Renderer/TexelBuffers.h"
#include "Utils/VulkanAllocator.h"


namespace Polyboid
{
	class VulkanTexelStorageBuffer : public TexelStorageBuffer
	{
	private:
		TexelBufferSettings m_Settings{};
		VulkanAllocator m_Allocator;
		vk::Buffer m_Buffer;
		vk::BufferView m_BufferView;
		vk::DescriptorBufferInfo m_BufferDescInfo{};
		uint32_t m_Size = 0;
	public:

		VulkanTexelStorageBuffer(const TexelBufferSettings& settings);
		vk::BufferView GetBufferView();
		void Init(const TexelBufferSettings& settings);

		std::any GetHandle() override;
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
		~VulkanTexelStorageBuffer() override;
		vk::DescriptorBufferInfo GetBufferDescInfo();
		void Destroy() override;
		void Recreate() override;
	};

	class VulkanTexelUniformBuffer : public TexelUniformBuffer
	{
	private:
		TexelBufferSettings m_Settings{};
		VulkanAllocator m_Allocator;
		vk::Buffer m_Buffer;
		vk::BufferView m_BufferView;
		vk::DescriptorBufferInfo m_BufferDescInfo{};
		uint32_t m_Size = 0;

		void Init(const TexelBufferSettings& settings);

	public:
		VulkanTexelUniformBuffer(const TexelBufferSettings& settings);

		vk::DescriptorBufferInfo GetBufferDescInfo();

		vk::BufferView GetBufferView();
		std::any GetHandle() override;
		EngineGraphicsFormats GetImageFormat() override;
		ImageLayout GetLayout() override;
		uint32_t GetHeight() override;
		uint32_t GetWidth() override;
		void Destroy() override;
		void Recreate() override;
	};

}

