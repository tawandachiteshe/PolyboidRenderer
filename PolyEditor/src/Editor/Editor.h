#pragma once
#include "EditorCamera.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	enum class EditorState
	{
		PLAY,
		EDIT,
		SIMULATE
	};

	struct EditorGlobalData
	{
		Ref<EditorCamera> m_EditorCamera;
		EditorState m_EditorState;
		std::string lastWorldPath;
		std::string lastWorldName;
	};

	class Editor
	{
	public:
		static Unique<EditorGlobalData>& GetEditorData() { return m_sEditorData; }
		static void SetEditorCamera(const Ref<EditorCamera>& camera) { m_sEditorData->m_EditorCamera = camera; }
		static Ref<EditorCamera>& GetEditorCamera() { return m_sEditorData->m_EditorCamera; }


		static void SetCurrentWorldMapPath(const std::string& name)
		{
			m_sEditorData->lastWorldPath = name;
		}

		static void SetCurrentWorldName(const std::string& name)
		{
			m_sEditorData->lastWorldName = name;
		}

		static std::string& GetCurrentWorldMapPath()
		{
			return  m_sEditorData->lastWorldPath;
		}

		static std::string& GetCurrentWorldName()
		{
			return m_sEditorData->lastWorldName;
		}




	private:
		static Unique<EditorGlobalData> m_sEditorData;


	};
	
}
