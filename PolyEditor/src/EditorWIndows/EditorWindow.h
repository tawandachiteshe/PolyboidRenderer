#pragma once

#include <string>

namespace Polyboid 
{
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

		virtual ~EditorWindow() = default;

	private:

	};
}