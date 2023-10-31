#include "OutlineWindow.h"

#include <spdlog/spdlog.h>

#include "imgui.h"
#include "Engine/Engine/Input.h"
#include "Engine/Engine/Math/Math.h"
#include "Engine/Engine/World/Components.h"
#include "Engine/Engine/World/GameObject.h"


namespace Polyboid
{
	OutlineWindow::OutlineWindow(const Ref<World>& world): m_World(world)
	{
		m_CurrentSelected = {};
	}

	void OutlineWindow::RenderOutliner()
	{
		ImGui::Begin("Outliner");
		DrawCreateNewGameObject();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);

		if (ImGui::TreeNode("World"))
		{
			m_World->m_Registry.each([&](entt::entity entity)
				{
					DrawGameObjectText(entity);
				});
			ImGui::TreePop();
		}
		ImGui::End();
	}

	void OutlineWindow::RenderGameObjectProperties()
	{
		ImGui::Begin("Properties");
		

		DrawComponent<TransformComponent>("Transform",[&](auto& component)
		{
			DrawTransformComponent(component);
		});

		ImGui::End();
	}

	void OutlineWindow::RenderUi()
	{
		bool show = true;

		ImGui::ShowDemoWindow(&show);
		RenderOutliner();
		RenderGameObjectProperties();

	}

	void OutlineWindow::DrawGameObjectText(entt::entity entity)
	{

		GameObject gameObject(entity, m_World.Get());
		const auto gameObjectString = gameObject.GetName();
		const auto gameObjectName = gameObjectString.c_str();
		const auto gameObjectID = gameObject.GetUUID();


		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;


		if (m_CurrentSelected.IsValid())
		{
			if (m_CurrentSelected.GetUUID() == gameObjectID)
			{
				baseFlags |= ImGuiTreeNodeFlags_Selected;
			}
		}

		if (ImGui::TreeNodeEx((void*)&gameObjectID, baseFlags, "%s", gameObjectName))
		{
			if (ImGui::IsItemClicked())
			{
				m_CurrentSelected = gameObject;
			}

			ImGui::TreePop();
		}
	}

	void OutlineWindow::DrawCreateNewGameObject()
	{
		const auto createObjectPopupID = "create_objects_popup";

		if (Input::MousePressed(MouseCodes::BUTTON_2))
			ImGui::OpenPopup(createObjectPopupID);

		if (ImGui::BeginPopup(createObjectPopupID))
		{
			if (ImGui::Selectable("Create GameObject"))
			{
				m_World->CreateGameObject("New GameObject");
			}

			ImGui::EndPopup();
		}
	}

	void OutlineWindow::DrawTransformComponent(TransformComponent& transform)
	{

		DrawVector("Position", transform.Position);
		ImGui::Separator();
		ImGui::Spacing();
		DrawVector("Scale", transform.Scale);
		ImGui::Separator();
		ImGui::Spacing();
		glm::vec3 rotation = glm::degrees(transform.Rotation);
		DrawVector("Rotation", rotation);
		transform.Rotation = glm::radians(rotation);
	}

	

	void OutlineWindow::DrawVector(const std::string& name, glm::vec3& value)
	{

		const auto width = ImGui::GetContentRegionAvail().x;
		const auto itemWidth = width / 3;


		ImGui::Text(name.c_str());

		ImGui::BeginTable(name.c_str(), 3);

		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::SetNextItemWidth(itemWidth);


		ImGui::PushStyleColor(ImGuiCol_FrameBg, { .98f, 0.24f, 0.3f, 1.0f });
		ImGui::DragFloat("X", &value.x);
		ImGui::PopStyleColor();

		ImGui::TableSetColumnIndex(1);
		ImGui::SetNextItemWidth(itemWidth);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.1f, 0.85f, 0.33f, 1.0f });
		ImGui::DragFloat("Y", &value.y);
		ImGui::PopStyleColor();


		ImGui::TableSetColumnIndex(2);
		ImGui::SetNextItemWidth(itemWidth);

		ImGui::PushStyleColor(ImGuiCol_FrameBg, { 0.1f, 0.45f, 0.90f, 1.0f });
		ImGui::DragFloat("Z", &value.z);
		ImGui::PopStyleColor();

		ImGui::EndTable();

	}
}
