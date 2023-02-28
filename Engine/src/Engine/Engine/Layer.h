#pragma once
#include <string>

namespace Polyboid 
{
	class Event;

	class Layer
	{
	public:
		Layer() = default;
		Layer(const std::string& name): m_Name(name), m_Active(true) {}
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnImguiRender() {}
		virtual void OnUpdate(float dt){}
		virtual void OnRender() {}
		virtual void OnEvent(Event& event) {}

		virtual ~Layer() = default;
		virtual std::string GetName() { return m_Name; }
		

	protected:
		std::string m_Name;
		bool m_Active{};

	};

}