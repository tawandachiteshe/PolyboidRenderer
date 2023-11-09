#pragma once
#include <glm/fwd.hpp>
#include <glm/vec3.hpp>

#include "imgui.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/World/GameObject.h"
#include "Engine/Engine/World/World.h"


namespace Polyboid
{
	struct TransformComponent;

	using DrawFunc = std::function<void()>;

	class OutlineWindow
	{
	public:
		OutlineWindow(const Ref<World>& world);

		void RenderOutliner();
		void RenderGameObjectProperties();


		void RenderUi();
		void DrawGameObjectText(entt::entity entity);
		void DrawCreateNewGameObject();
		static void DrawTransformComponent(TransformComponent& transform);

		template <typename Component>
		void DrawComponent(const std::string& name, const std::function<void(Component& component)>& func)
		{
			constexpr ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;

			if (ImGui::CollapsingHeader(name.c_str(), flags))
			{
				DrawIfComponentExist<TransformComponent>(m_CurrentSelected, [&](TransformComponent& component)
				{
					func(component);
				});
			}
		}

		template <typename T>
		void DrawIfComponentExist(GameObject& object,
		                          const std::function<void(T& component)>& func)
		{
			if (!object.IsValid())
				return;

			if (object.HasComponent<T>())
			{
				auto& component = object.GetComponent<T>();
				func(component);
			}
		}

	private:
		Ref<World> m_World;
		GameObject m_CurrentSelected{};

	private:
		static void DrawVector(const std::string& name, glm::vec3& value);
	};
}
