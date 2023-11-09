#include "boidpch.h"
#include "VkDebugMessenger.h"

#include "Common.h"
#include "Engine/Engine/Debug/Logger.h"


namespace Polyboid
{

	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData) {


		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
			// Message is important enough to show
			Logger::GetLogger()->warn("{}", pCallbackData->pMessage);
		}

		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
		{
			Logger::GetLogger()->error("{}", pCallbackData->pMessage);
		}

		if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		{
			Logger::GetLogger()->info("{}", pCallbackData->pMessage);
		}

		

		return VK_FALSE;
	}

	VkDebugMessenger::VkDebugMessenger(const Ref<VkInstance>& instance)
	{
		vk::Result result;
		auto m_Instance = instance->GetVKInstance();
		auto dldi = instance->GetLoader();

		vk::DebugUtilsMessengerCreateInfoEXT debugInfo{};
		debugInfo.sType = vk::StructureType::eDebugUtilsMessengerCreateInfoEXT;

		using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
		using enum vk::DebugUtilsMessageTypeFlagBitsEXT;
		debugInfo.flags = vk::DebugUtilsMessengerCreateFlagsEXT();
		debugInfo.messageSeverity = eVerbose | eWarning | eError | eInfo;
		debugInfo.messageType = eGeneral | eValidation | ePerformance;
		debugInfo.pfnUserCallback = debugCallback;

		auto[messengerResult, messenger] = m_Instance.createDebugUtilsMessengerEXT(debugInfo, nullptr, dldi);
		vk::resultCheck(messengerResult, "Failed to create messenger");
		m_Messenger = messenger;

	}

	VkDebugMessenger::~VkDebugMessenger()
	{


	}
}
