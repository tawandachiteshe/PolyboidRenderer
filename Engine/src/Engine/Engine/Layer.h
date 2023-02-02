#pragma once
#include <string>

namespace Polyboid 
{
	class Layer
	{
	public:
		Layer() = default;
		Layer(const std::string& name): m_Name(name), m_Active(true) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImguiRender() {}
		virtual void OnUpdate(float dt){}
		virtual void OnRender(float dt) {}

		virtual ~Layer() {};
		virtual std::string GetName() { return m_Name; }
		

	protected:
		std::string m_Name;
		bool m_Active;

	};

}