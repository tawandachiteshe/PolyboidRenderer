#include "DetailsWindow.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "Engine/Engine/ECS/Components.h"
#include "Engine/Engine/Events/EventDispatcher.h"
#include "Engine/Engine/Scripting/ScriptingEngine.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "glm/gtc/type_ptr.hpp"

namespace Polyboid
{
	void DetailsWindow::OnGameObjectSelected(const Event& event)
	{
		m_CurrentObject = nullptr;
	}

	void DetailsWindow::OnGameObjectDeleted(const Event& event)
	{
		m_CurrentObject = nullptr;
	}

	DetailsWindow::DetailsWindow(const std::string& name)
	{
		m_Name = name;
	}

	static bool DrawColoredButton(const char* text, const glm::vec4& color, std::string& name)
	{
		std::string makeID = name + text;

		ImGui::PushID(makeID.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, {color.x, color.y, color.z, color.w});
		const bool isClicked = ImGui::Button(text);
		ImGui::PopStyleColor();
		ImGui::PopID();

		return isClicked;
	}


	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f,
	                            float columnWidth = 100.0f)
	{
		ImGuiIO& io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();
	}

	static void DrawTransformVectorComponent(const std::string& label, glm::vec3& value, float size = 68)
	{
		static int index = 0;


		auto makeIDX = std::string("##PX").append(label);
		auto makeIDY = std::string("##PY").append(label);
		auto makeIDZ = std::string("##PZ").append(label);


		ImGui::Text(label.c_str());
		ImGui::SameLine();
		ImGui::DragFloat("X", &value.x, 0.01f);
		ImGui::DragFloat("Y", &value.y, 0.01f);
		ImGui::DragFloat("Z", &value.z, 0.01f);

		//ImGui::DragFloat3(label.c_str(), glm::value_ptr(value), 0.01f);
	}

	static void DrawTransformComponent(GameObject* gameObject, float size)
	{
		auto& transform = gameObject->GetComponent<TransformComponent>();

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//Position
			DrawVec3Control("Position", transform.Position);

			//Scale
			DrawVec3Control("Scale", transform.Scale, 1.0f);

			//Rotation
			glm::vec3 rotation = glm::degrees(transform.Rotation);
			DrawVec3Control("Rotation", rotation);
			transform.Rotation = glm::radians(rotation);
			//transform.Rotation = glm::radians(rotation);
		}
	}

	void DetailsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());


		ImGui::End();
	}

	void DetailsWindow::Update(float ts)
	{
	}

	DetailsWindow::~DetailsWindow()
	{
	}
}
