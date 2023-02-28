#pragma once

#include <string>

namespace Polyboid 
{
	class Event;
	// window data maybe
	// entity compontnt systeam data
	// maybe camera data but for hwa????

	class EditorWindow
	{

	protected:

		std::string m_Name;

	public:
		EditorWindow() = default;

		virtual void RenderImgui() = 0;
		virtual void Update(float ts) = 0;
		virtual void OnRender() {}
		virtual void OnEvent(Event& event) {}

		virtual ~EditorWindow() = default;

	private:

	};
}