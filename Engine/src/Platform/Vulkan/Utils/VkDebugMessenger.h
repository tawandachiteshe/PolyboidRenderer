#pragma once
#include "VkInstance.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class VkDebugMessenger
	{
	private:
		vk::DebugUtilsMessengerEXT m_Messenger;
	public:

		vk::DebugUtilsMessengerEXT& GetMessenger()
		{
			return m_Messenger;
		}

		VkDebugMessenger(const Ref<VkInstance>& instance);
		~VkDebugMessenger();

	};

}


