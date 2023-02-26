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

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }


		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }
		

		std::vector<Layer*>& GetLayers() { return m_Layers; }

	private:

		std::vector<Layer*> m_Layers;

	};
}
