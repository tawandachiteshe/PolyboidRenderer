#pragma once
#include <cstdint>

#include "Engine/Core/Base.h"
#include "Engine/Core/ECS/ECSManager.h"

namespace Polyboid
{
    class GameObject
    {
    private:
        uint32_t m_ID;
        std::string m_Name;
    public:
        GameObject(const std::string& name = "GameObject");
        static Ref<GameObject> CreateObject();

        template <typename Component, typename ... Args>
        void AddComponent(Args&&... args);

        template <class  Component>
        Component& GetComponent();
    };

    template <class  Component>
    Component& GameObject::GetComponent()
    {
        
        auto view = ECSManager::GetRegistry().view<Component>();
        return view.get<Component>(static_cast<const entt::entity>(m_ID));
    
    }
}
