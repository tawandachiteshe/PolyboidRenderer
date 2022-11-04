#pragma once
#include <cstdint>

#include "Components.h"
#include "Engine/Engine/Gameplay/World.h"
#include "entt/entt.hpp"

namespace Polyboid
{
	class GameObject
    {

    protected:
        World* m_World = nullptr;
        std::string m_Name = "";
        uint32_t m_ID = entt::null;
        World* GetWorld() const { return  m_World; }

    public:
        virtual ~GameObject() = default;

        virtual void OnGameObjectConstruction()
        {
            m_ID = GetWorld()->CreateEntityID();

            m_Name = "GameObject";

            if (m_Name == "GameObject")
            {
                m_Name += "_" + std::to_string(m_ID);
            }

            AddComponent<Tag>(m_Name);
            AddComponent<Transform>();
        }
        virtual void OnBeginPlay() {}
        virtual void OnEndPlay() { }
        virtual void OnUpdate(float dt) { }

    public:

      

        GameObject(const std::string& name = "GameObject"): m_Name(name) {}

        template <typename Component, typename ... Args>
        void AddComponent(Args&&... args);

        template <typename Component, typename ... Args>
        void AddOrReplaceComponent(Args&&... args);

        template <class  Component>
        Component& GetComponent();

        template <class  Component>
        bool RemoveComponent();

        template<class Component>
        bool HasComponent();

    };

    template <class  Component>
    Component& GameObject::GetComponent()
    {
        auto& registry = GetWorld()->GetRegistry();

        auto view = registry.view<Component>();
        
        return view.get<Component>(static_cast<const entt::entity>(m_ID));
    
    }

    template <typename Component, typename ... Args>
    void GameObject::AddComponent(Args&&... args)
    {
        auto& registry = GetWorld()->GetRegistry();
        registry.emplace<Component>(static_cast<const entt::entity>(m_ID), std::forward<Args>(args) ...);
    }

    template <typename Component, typename ... Args>
    void GameObject::AddOrReplaceComponent(Args&&... args)
    {
        auto& registry = GetWorld()->GetRegistry();
        registry.emplace_or_replace<Component>(static_cast<const entt::entity>(m_ID), std::forward<Args>(args) ...);
    }

    template <typename Component>
    bool GameObject::RemoveComponent()
    {
        auto& registry = GetWorld()->GetRegistry();

        registry.clear<Component>();
    	registry.remove<Component>(static_cast<const entt::entity>(m_ID));
      
        return false;
    }

    template <class Component>
    bool GameObject::HasComponent()
    {
        auto& registry = GetWorld()->GetRegistry();

        bool isThere = registry.any_of<Component>(static_cast<const entt::entity>(m_ID));

        return isThere;
    }



}
