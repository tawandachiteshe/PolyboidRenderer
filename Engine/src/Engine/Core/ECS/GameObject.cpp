#include "GameObject.h"

#include "Components.h"
#include "ECSManager.h"



namespace Polyboid
{

    GameObject::GameObject(const std::string& name): m_ID(0), m_Name(name)
    {
        
        m_ID = ECSManager::CreateEntity();

        if (m_Name == "GameObject")
        {
            m_Name += "_" + std::to_string(m_ID);
        }
        
        AddComponent<Tag>(m_Name);
        AddComponent<Transform>();
    }

    Ref<GameObject> GameObject::CreateObject()
    {
        return std::make_shared<GameObject>(); 
    }

    template <typename Component, typename ... Args>
    void GameObject::AddComponent(Args&&... args)
    {
        auto& registry = ECSManager::GetRegistry();
        registry.emplace<Component>(static_cast<const entt::entity>(m_ID), std::forward<Args>(args) ...);
    }



    // template <typename Component>
    // Component& GameObject::GetComponent()
    // {
    //     auto view = ECSManager::GetRegistry().view<const Component>();
    //     return view[0].get<Component>(static_cast<const entt::entity>(m_ID));
    // }
}
