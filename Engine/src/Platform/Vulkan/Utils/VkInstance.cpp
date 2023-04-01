#include "boidpch.h"
#include "VkInstance.h"

#include "Common.h"
#include "GLFW/glfw3.h"
#include "spdlog/spdlog.h"


namespace Polyboid
{


	void VkInstance::LogAndGetExtensions(std::vector<vk::ExtensionProperties>& props)
	{
		auto [extensionsResult, extensions] = vk::enumerateInstanceExtensionProperties();
		vk::resultCheck(extensionsResult, "Failed to get extensions");

		spdlog::info("Available Extensions:");
		for (const auto& extension : extensions)
		{
			spdlog::info("{}", extension.extensionName);
		}
	}

	void VkInstance::LogAndGetLayers(std::vector<vk::LayerProperties>& layers)
	{

		auto [availableResults, availableLayers] = vk::enumerateInstanceLayerProperties();
		vk::resultCheck(availableResults, "Failed to get layer properties");

		spdlog::info("Available Layers: ");

		std::map<std::string, uint32_t> layerMap;

		uint32_t count = 0;
		for (const auto& layer : availableLayers)
		{
			spdlog::info("{}", layer.layerName);

			layerMap[layer.layerName] = count;

			count++;
		}


		if (layerMap.contains("VK_LAYER_KHRONOS_validation"))
		{
			spdlog::info("Layer {} requested is available. ", "VK_LAYER_KHRONOS_validation");
		}
		else
		{
			__debugbreak();
		}
	}

	VkInstance::VkInstance()
	{
		const std::vector<const char*> validationLayers = {
			"VK_LAYER_KHRONOS_validation"
		};

		std::vector<const char*> extensions = {
			"VK_EXT_debug_utils"
		};

		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = nullptr;

		glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

		if (glfwExtensions == nullptr)
		{
			__debugbreak();
		}

		for (uint32_t i = 0; i < glfwExtensionCount; ++i)
		{
			extensions.push_back(glfwExtensions[i]);
			spdlog::info("Supported GLFW extensions {}", glfwExtensions[i]);
		}

		vk::ApplicationInfo applicationInfo;
		
		applicationInfo.sType = vk::StructureType::eApplicationInfo;
		applicationInfo.pApplicationName = "Polyboid Application";
		applicationInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.pEngineName = "Polyboid Engine";
		applicationInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		applicationInfo.apiVersion = VK_API_VERSION_1_2;

		vk::InstanceCreateInfo createInfo{};
		createInfo.flags = vk::InstanceCreateFlags();
		createInfo.sType = vk::StructureType::eInstanceCreateInfo;
		createInfo.pApplicationInfo = &applicationInfo;

		std::vector<vk::ExtensionProperties> props;
		LogAndGetExtensions(props);

		std::vector<vk::LayerProperties> layers;
		LogAndGetLayers(layers);

		createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
		createInfo.ppEnabledExtensionNames = extensions.data();
		createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
		createInfo.ppEnabledLayerNames = validationLayers.data();

		vk::Result result = vk::Result::eErrorUnknown;
		result = vk::createInstance(&createInfo, nullptr, &m_Instance);

		CheckResult(result, "Successfully created Vulkan Instance")

		m_Dldi = vk::DispatchLoaderDynamic(m_Instance, vkGetInstanceProcAddr);

	}

	VkInstance::~VkInstance()
	{
	}
}
