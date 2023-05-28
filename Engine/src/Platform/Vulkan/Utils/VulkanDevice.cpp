#include "boidpch.h"
#include "VulkanDevice.h"

#include <set>

#include "VulkanPhysicalDevice.h"


namespace Polyboid
{
	VulkanDevice::VulkanDevice(const Ref<VulkanPhysicalDevice>& device)
	{
		auto indices = device->GetFamilyIndices();
		auto deviceFeatures = device->GetPhysicalDevice().getFeatures2();
		auto physicalDevice = device->GetPhysicalDevice();


		std::vector<const char*> deviceExtensions
		{ VK_KHR_SWAPCHAIN_EXTENSION_NAME };

		const std::vector<const char*> validationLayers = 
		{ "VK_LAYER_KHRONOS_validation"};

		std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;

		std::set uniqueQueueFamilies = { indices.GraphicsFamily.value(), indices.PresentFamily.value() };

		for (auto queueFamily : uniqueQueueFamilies)
		{
			vk::DeviceQueueCreateInfo queueCreateInfo{};
			queueCreateInfo.flags = vk::DeviceQueueCreateFlags();
			queueCreateInfo.sType = vk::StructureType::eDeviceQueueCreateInfo;
			queueCreateInfo.queueFamilyIndex = queueFamily;
			queueCreateInfo.queueCount = 1;
			
			float priority = 1.0f;
			queueCreateInfo.pQueuePriorities = &priority;

			queueCreateInfos.push_back(queueCreateInfo);
		}



		vk::DeviceCreateInfo createInfo{};
		createInfo.flags = vk::DeviceCreateFlags();
		createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
		createInfo.setPQueueCreateInfos(queueCreateInfos.data());
		createInfo.setPEnabledExtensionNames(deviceExtensions);
		createInfo.setPEnabledLayerNames(validationLayers);

		auto [result, logicalDevice] = physicalDevice.createDevice(createInfo);
		vk::resultCheck(result, "Failed to create device");
		m_Device = logicalDevice;

		m_PresentQueue = m_Device.getQueue(indices.PresentFamily.value(), 0);
		m_GraphicsQueue = m_Device.getQueue(indices.GraphicsFamily.value(), 0);

	}

}
