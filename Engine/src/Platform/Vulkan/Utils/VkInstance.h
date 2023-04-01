#pragma once
#include <vulkan/vulkan.hpp>


namespace Polyboid
{
	class VkInstance
	{
	private:
		vk::Instance m_Instance;

		vk::DispatchLoaderDynamic m_Dldi;

		void LogAndGetExtensions(std::vector<vk::ExtensionProperties>& props);

		void LogAndGetLayers(std::vector<vk::LayerProperties>& layers);


	public:

		operator vk::Instance() const { return m_Instance; }
		operator vk::Instance() { return m_Instance; }

		vk::Instance& GetVKInstance() { return m_Instance; }

		vk::DispatchLoaderDynamic GetLoader()
		{
			return m_Dldi;
		}

		VkInstance();
		~VkInstance();
	};
}
