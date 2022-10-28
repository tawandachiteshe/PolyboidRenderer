#pragma once
#include <Engine/Engine/Layer.h>
#include <vector>


namespace Polyboid
{

	class LayerContainer
	{
	public:
		LayerContainer() = default;
		void AddLayer(Layer* layer)
		{
			layer->OnAttach();
			m_Layers.emplace_back(layer);
		}

		void RemoveLayer(Layer* layer)
		{
			layer->OnDetach();
			auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
			m_Layers.erase(it);
		}

		std::vector<Layer*>& GetLayers() { return m_Layers; }

	private:

		std::vector<Layer*> m_Layers;

	};
}