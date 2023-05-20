#include "boidpch.h"
#include "VulkanPhysicalDevice.h"

#include <spdlog/spdlog.h>

#include "VkInstance.h"
#include "VulkanSurfaceKHR.h"


namespace Polyboid
{

	VulkanPhysicalDevice::VulkanPhysicalDevice(const Ref<VkInstance>& _instance, const Ref<VulkanSurfaceKHR>& surface)
	{

		vk::Instance instance = (*_instance);


		auto [result, devices] = instance.enumeratePhysicalDevices();
		vk::resultCheck(result, "failed to enum physical devices");

		spdlog::info("They are {} physical devices", devices.size());

		std::map<vk::PhysicalDeviceType, vk::PhysicalDevice> deviceMap;

		for (const auto & device : devices)
		{

			auto props = device.getProperties();

			spdlog::info("Device name: {}",
				props.deviceName);

			spdlog::info("Vulkan: {}.{}.{}",
				VK_API_VERSION_MAJOR(props.apiVersion), 
				VK_API_VERSION_MINOR(props.apiVersion),
				VK_API_VERSION_PATCH(props.apiVersion));


			switch (props.deviceType)	  
			{
				case vk::PhysicalDeviceType::eOther: spdlog::info("Device type: Other"); break;
				case vk::PhysicalDeviceType::eIntegratedGpu: spdlog::info("Device type: Integrated Gpu"); break;
				case vk::PhysicalDeviceType::eDiscreteGpu: spdlog::info("Device type: Discrete Gpu"); break;
				case vk::PhysicalDeviceType::eVirtualGpu: spdlog::info("Device type: Virtual Gpu"); break;
				case vk::PhysicalDeviceType::eCpu: spdlog::info("Device type: Cpu \n"); break;
			}

			//I assume there is only one discrete gpu thats what i select;

			deviceMap[props.deviceType] = device;

		}

		if (deviceMap.contains(vk::PhysicalDeviceType::eDiscreteGpu))
		{
			m_Device = deviceMap[vk::PhysicalDeviceType::eDiscreteGpu];
		}
		else if (deviceMap.contains(vk::PhysicalDeviceType::eIntegratedGpu))
		{
			m_Device = deviceMap[vk::PhysicalDeviceType::eIntegratedGpu];
		}
		else
		{

			spdlog::error("Device not supported");
			__debugbreak();
		}


		//Finding queues

		auto queueProps = m_Device.getQueueFamilyProperties();


		uint32_t count = 0;
		for (const auto& queueProp : queueProps)
		{

			if(queueProp.queueFlags & vk::QueueFlagBits::eGraphics)
			{
				m_Indices.GraphicsFamily = count;
			}


			vk::Bool32 presentSupport = false;
			auto _result = m_Device.getSurfaceSupportKHR(count, (*surface), &presentSupport);

			if (_result == vk::Result::eSuccess)
			{
				if (presentSupport)
				{
					m_Indices.PresentFamily = count;
				}

			}

			if (m_Indices.IsComplete())
			{
				break;
			}

			count++;
		}

		if (!m_Indices.IsComplete())
		{
			m_Device = nullptr;
			spdlog::error("Device is not complete");
			__debugbreak();
		}

		const auto [extenResults, extensions] = m_Device.enumerateDeviceExtensionProperties();
		vk::resultCheck(extenResults, "Failed to enum device properties");

		count = 0;

		std::map<std::string, uint32_t> extensionMap;
		for (const auto& extension : extensions)
		{
			extensionMap[extension.extensionName] = count;
			count++;

		}

		if (extensionMap.contains(VK_KHR_SWAPCHAIN_EXTENSION_NAME))
		{
			spdlog::info("Supports {} extension", VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		}
		else
		{
			spdlog::error("Does not support swapchain");
			__debugbreak();
		}


		spdlog::info("Device successfully selected.");

	}
}
