#include "DetailsWindow.h"

#include "imgui.h"
#include "Editor/Events/EditorEvents.h"
#include "Engine/Engine/AssetManager.h"
#include "Engine/Engine/Events/EventSystem.h"
#include "Engine/Engine/Scripting/ScriptingEngine.h"
#include "Engine/Renderer/MaterialLibrary.h"
#include "glm/gtc/type_ptr.hpp"

namespace Polyboid
{
	void DetailsWindow::OnGameObjectSelected(const Event& event)
	{
		auto gameObjectSelected = CastEventAs<GameObjectOutlineClick>(event);
		m_CurrentObject = nullptr;
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
		ImGui::PushStyleColor(ImGuiCol_Button, { color.x, color.y, color.z, color.w });
		const bool isClicked = ImGui::Button(text, { 32, 0 });
		ImGui::PopStyleColor();
		ImGui::PopID();

		return isClicked;
	}

	static void DrawTransformVectorComponent(const std::string& label, glm::vec3& value, float size = 68)
	{
		static int index = 0;


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
		DrawColoredButton("Y", { 0.21f, 0.86f, 0.11f, 1.0f }, makeIDY);
		ImGui::SameLine();
		ImGui::SetNextItemWidth(size);


		ImGui::DragFloat(makeIDY.c_str(), &value.y, 0.01f);
		ImGui::SameLine();
		DrawColoredButton("Z", { 0.0f, 0.0f, 1.0f, 1.0f }, makeIDZ);
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
			static auto rotation = transform.Rotation;
			DrawTransformVectorComponent("Rotation", transform.Rotation, size);
			//transform.Rotation = glm::radians(rotation);
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
			ImGui::Button("Add Script");

			if (ImGui::BeginPopupContextItem("AddScript", ImGuiPopupFlags_MouseButtonLeft))
			{
				for (auto& monoClass : ScriptingEngine::GetClasses())
				{
					std::string className = monoClass;
					if (ImGui::Selectable(className.c_str()))
					{
						m_CurrentObject->AttachScript(className);
					}
				}
				ImGui::EndPopup();
			}

			ImGui::Button("Add Component", { windowContent - 4, 0 });


			if (ImGui::BeginPopupContextItem("##item", ImGuiPopupFlags_MouseButtonLeft))
			{
				ImGui::SetNextItemWidth(windowContent);

				if (!m_CurrentObject->HasComponent<ShapeComponent>())
				{
					if (ImGui::Selectable("Shape"))
					{
						m_CurrentObject->AddComponent<ShapeComponent>();
					}
				}

				if (!m_CurrentObject->HasComponent<MeshRendererComponent>())
				{
					if (ImGui::Selectable("Mesh Renderer"))
					{
						UUID id;
						m_CurrentObject->AddComponent<MeshRendererComponent>(id);
						MaterialLibrary::CreateMaterial(id, "Default");
					}
				}

				if (!m_CurrentObject->HasComponent<LightComponent>())
				{
					if (ImGui::Selectable("Light"))
					{
						m_CurrentObject->AddComponent<LightComponent>();
					}
				}

				if (!m_CurrentObject->HasComponent<PointLightComponent>())
				{
					if (ImGui::Selectable("Point Light"))
					{
						m_CurrentObject->AddComponent<PointLightComponent>();
					}
				}

				if (!m_CurrentObject->HasComponent<SpotLightComponent>())
				{
					if (ImGui::Selectable("Spot Light"))
					{
						auto& transform = m_CurrentObject->GetComponent<TransformComponent>();
						transform.Rotation = { 0, -1, 0 };
						m_CurrentObject->AddComponent<SpotLightComponent>();
					}
				}


				if (!m_CurrentObject->HasComponent<DirectionLightComponent>())
				{
					if (ImGui::Selectable("Directional Light"))
					{
						auto& transform = m_CurrentObject->GetComponent<TransformComponent>();
						transform.Rotation = { 0, -1, 0 };
						m_CurrentObject->AddComponent<DirectionLightComponent>();
					}
				}


				ImGui::EndPopup();
			}

			ImGui::Spacing();
			ImGui::Spacing();
			ImGui::Spacing();

			if (m_CurrentObject->HasComponent<TransformComponent>())
			{
				DrawTransformComponent(m_CurrentObject, size);
			}


			if (m_CurrentObject->HasComponent<ShapeComponent>())
			{
				ImGui::Spacing();
				ImGui::Separator();

				if (ImGui::CollapsingHeader("Shape", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& shapeComponent = m_CurrentObject->GetComponent<ShapeComponent>();

					ImGui::ColorEdit3("Color", glm::value_ptr(shapeComponent.color));


					const char* items[] = {
						"QUAD", "CIRCLE", "LINE"
					};
					static int item_current = 0;
					if (ImGui::Combo("Shape Type", &item_current, items, IM_ARRAYSIZE(items)))
					{
						switch (item_current)
						{
						case 0:
							shapeComponent.type = ShapeType::Quad;
							break;
						case 1:
							shapeComponent.type = ShapeType::Circle;
							break;
						default:
							break;
						}
					}
					if (shapeComponent.type == ShapeType::Circle)
					{
						ImGui::DragFloat("Thickness", &shapeComponent.thickness, 0.01f);
						ImGui::DragFloat("Fade", &shapeComponent.fade, 0.01f);
					}
				}
			}


			if (m_CurrentObject->HasComponent<DirectionLightComponent>())
			{

				if (ImGui::CollapsingHeader("Directional Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& light = m_CurrentObject->GetComponent<DirectionLightComponent>();

					if (ImGui::ColorEdit3("Color", glm::value_ptr(light.color)))
					{
						
					}

					if (ImGui::DragFloat3("Direction", glm::value_ptr(light.direction)))
					{

					}

					ImGui::DragFloat("Energy", &light.Energy, 0.01f);

				}
			}


			if (m_CurrentObject->HasComponent<PointLightComponent>())
			{

				if (ImGui::CollapsingHeader("Point Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& light = m_CurrentObject->GetComponent<PointLightComponent>();

					ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
					ImGui::DragFloat("Distance", &light.Distance, 0.01f);
					ImGui::DragFloat("Energy", &light.Energy, 0.01f);

				}
			}



			if (m_CurrentObject->HasComponent<SpotLightComponent>())
			{

				if (ImGui::CollapsingHeader("Spot Light", ImGuiTreeNodeFlags_DefaultOpen))
				{
					auto& light = m_CurrentObject->GetComponent<SpotLightComponent>();

					if (ImGui::ColorEdit3("Color", glm::value_ptr(light.color)))
					{

					}

					if (ImGui::DragFloat3("Direction", glm::value_ptr(light.direction)))
					{

					}

					if (ImGui::DragFloat("Inner Angle", &light.InnerAngle, 0.01f))
					{

					}


					if (ImGui::DragFloat("Outer Angle", &light.OuterAngle, 0.01f))
					{

					}

					ImGui::DragFloat("Energy", &light.Energy, 0.01f);



				}
			}

			if (m_CurrentObject->HasComponent<MeshRendererComponent>())
			{

				if (ImGui::CollapsingHeader("Mesh Renderer", ImGuiTreeNodeFlags_DefaultOpen))
				{

					auto& meshRenderer = m_CurrentObject->GetComponent<MeshRendererComponent>();

					static int item_current = 0;

					auto meshes = AssetManager::LoadedMeshes();

					static int currentItem = 0;

					if(ImGui::BeginCombo("Mesh Asset", meshes[currentItem].c_str()))
					{

						for (int n = 0; n < meshes.size(); n++)
						{
							const bool is_selected = (currentItem == n);
							if (ImGui::Selectable(meshes[n].c_str(), is_selected))
							{
								currentItem = n;
								meshRenderer.assetName = meshes[currentItem];
							}

						}
						ImGui::EndCombo();
					}

					if (auto mat = MaterialLibrary::GetMaterial(meshRenderer.assetId))
					{
						static auto albedoColor = mat->GetAlbedo();
						if(ImGui::ColorEdit3("Albedo Color", glm::value_ptr(albedoColor)))
						{
							mat->SetAlbedo(albedoColor);
						}

						static auto specularColor = mat->GetAO();
						if (ImGui::ColorEdit3("Ambient Occlusion", glm::value_ptr(specularColor)))
						{
							mat->SetAO(specularColor);
						}

						static auto roughness = mat->GetRoughness();
						if (ImGui::DragFloat("Roughness", &roughness, 0.01f, 0.0f, 1.0f))
						{
							mat->SetRoughness(roughness);
						}

						static auto metallic = mat->GetMetallic();
						if (ImGui::DragFloat("Metallic", &metallic, 0.01f, 0.0f, 1.0f))
						{
							mat->SetMetallic(roughness);
						}
					}

					if (ImGui::Button("Add Standard Material"))
					{
						//MaterialLibrary::CreateMaterial(meshRenderer.assetId, "Default");
					}
				}
			}

			if (m_CurrentObject->HasScriptsAttached())
			{
				for (auto& script : m_CurrentObject->GetMonoScripts())
				{
					std::string scriptName = script->GetMonoClassName();

					if (ImGui::CollapsingHeader(scriptName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
					{
						for (auto& field : script->GetFields())
						{
							const auto& [name, type] = field;

							switch (type)
							{
							case ScriptingType::Boolean:
							{
								auto value = script->GetField<bool>(name);
								if (ImGui::Checkbox(name.c_str(), &value))
								{
									script->SetField<bool>(name, value);
								}
							}
							break;
							case ScriptingType::Vector3:
							{
								auto value = script->GetField<glm::vec3>(name);
								if (ImGui::DragFloat3(field.first.c_str(), glm::value_ptr(value), .1f))
								{
									script->SetField<glm::vec3>(name, value);
								}
								break;
							}

							case ScriptingType::Float:
							{
								float value = script->GetField<float>(name);
								if (ImGui::DragFloat(field.first.c_str(), &value, .1f))
								{
									script->SetField<float>(name, value);
								}
							}

							break;
							default:
								break;
							}
						}
					}
				}
			}

		}


		ImGui::End();
	}

	void DetailsWindow::Update(float ts)
	{
	}

	DetailsWindow::~DetailsWindow()
	{
	}

}


