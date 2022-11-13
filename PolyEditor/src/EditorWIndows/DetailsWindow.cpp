#include "DetailsWindow.h"

#include "imgui.h"
#include "Editor/Events/EditorEvents.h"
#include "Engine/Engine/Events/EventSystem.h"
#include "glm/gtc/type_ptr.hpp"

namespace Polyboid
{
	void DetailsWindow::OnGameObjectSelected(const Event& event)
	{
		auto gameObjectSelected = CastEventAs<GameObjectOutlineClick>(event);
		m_CurrentObject = gameObjectSelected.GetGameObject();
	}

	void DetailsWindow::OnGameObjectDeleted(const Event& event)
	{
		m_CurrentObject = nullptr;
	}

	DetailsWindow::DetailsWindow(const std::string& name)
	{
		m_Name = name;
		EventSystem::Bind(EventType::ON_GAME_OBJECT_SELECTED, BIND_EVENT(OnGameObjectSelected));
		EventSystem::Bind(EventType::ON_GAME_OBJECT_DELETED, BIND_EVENT(OnGameObjectDeleted));
	}

	static bool DrawColoredButton(const char* text, const glm::vec4& color, std::string& name)
	{
		std::string makeID = name + text;

		ImGui::PushID(makeID.c_str());
		ImGui::PushStyleColor(ImGuiCol_Button, {color.x, color.y, color.z, color.w});
		const bool isClicked = ImGui::Button(text, { 32, 0 });
		ImGui::PopStyleColor();
		ImGui::PopID();

		return isClicked;
	}

	static void DrawTransformVectorComponent(const std::string& label, glm::vec3& value, float size = 68)
	{
		static  int index = 0;
	
		auto makeIDX = std::string("##PX").append(label);
		auto makeIDY = std::string("##PY").append(label);
		auto makeIDZ = std::string("##PZ").append(label);


		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, { 0, 10 });
		ImGui::TextWrapped(label.c_str());
		DrawColoredButton("X", { 1.0f, 0.0f, 0.0f, 1.0f }, makeIDX);


		ImGui::SameLine();
		ImGui::SetNextItemWidth(size);
		

		ImGui::DragFloat(makeIDX.c_str(), &value.x, 0.01f);
		ImGui::SameLine();
		DrawColoredButton("Y", {0.21f, 0.86f, 0.11f, 1.0f}, makeIDY);
	
		ImGui::SameLine();
		ImGui::SetNextItemWidth(size);


		ImGui::DragFloat(makeIDY.c_str(), &value.y, 0.01f);
		ImGui::SameLine();
		DrawColoredButton("Z", {0.0f, 0.0f, 1.0f, 1.0f}, makeIDZ);
		ImGui::SameLine();

		
		ImGui::SetNextItemWidth(size);
		ImGui::DragFloat(makeIDZ.c_str(), &value.z, 0.01f);
		ImGui::PopStyleVar();


	}

	static void DrawTransformComponent(GameObject* gameObject, float size)
	{
		auto& transform = gameObject->GetComponent<TransformComponent>();

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			//Position
			DrawTransformVectorComponent("Position", transform.Position, size);

			//Scale
			DrawTransformVectorComponent("Scale", transform.Scale, size);

			//Rotation
			DrawTransformVectorComponent("Rotation", transform.Rotation, size);


		}
	}

	void DetailsWindow::RenderImgui()
	{
		ImGui::Begin(m_Name.c_str());

		static float spacing = 12;

		const float windowContent = ImGui::GetWindowContentRegionMax().x;

		float size = (windowContent / 3) - 34;


		if (m_CurrentObject != nullptr)
		{

			ImGui::Button("Add Component", { windowContent - 4, 0 });


			if (ImGui::BeginPopupContextItem("##item", ImGuiPopupFlags_MouseButtonLeft))
			{
				ImGui::SetNextItemWidth(windowContent);
				if (ImGui::Selectable("Shape"))
				{

				}

				ImGui::EndPopup();
			}


			static glm::vec3 position;
			static glm::vec3 scale;
			static glm::vec3 rotation;

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			if (m_CurrentObject->HasComponent<TransformComponent>())
			{
				DrawTransformComponent(m_CurrentObject, size);
			}



			if (m_CurrentObject->HasComponent<ShapeComponent>());
			{
				ImGui::Spacing();
				ImGui::Separator();


				if (ImGui::CollapsingHeader("Shape", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& shapeComponent = m_CurrentObject->GetComponent<ShapeComponent>();
					ImGui::ColorEdit3("Color", glm::value_ptr(shapeComponent.color));
				}
				



			}
		}


	
		ImGui::Separator();





		ImGui::End();
	}

	void DetailsWindow::Update(float ts)
	{
	}

	DetailsWindow::~DetailsWindow()
	{
	}
}
