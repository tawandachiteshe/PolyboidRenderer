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
	};

	class Editor
	{
	public:
		static Unique<EditorGlobalData>& GetEditorData() { return m_sEditorData; }
		static void SetEditorCamera(const Ref<EditorCamera>& camera) { m_sEditorData->m_EditorCamera = camera; }
		static Ref<EditorCamera>& GetEditorCamera() { return m_sEditorData->m_EditorCamera; }
	private:
		static Unique<EditorGlobalData> m_sEditorData;


	};
	
}
