#pragma once
#include <cstdint>
#include <spdlog/spdlog.h>

#include "Engine/Engine/Base.h"
#include "Engine/Engine/Gameplay/World.h"
#include "entt/entt.hpp"

namespace Polyboid
{
   
	class GameObject
    {

    protected:
        Ref<World> m_World = nullptr;
        std::string m_Name = "";
        uint32_t m_ID = entt::null;
		Ref<World> GetWorld() { return  m_World; }

    public:
        virtual ~GameObject();
        virtual void OnCreate() {}
        virtual void OnBeginPlay() {}
        virtual void OnEndPlay() { }
        virtual void OnDestroy();
        virtual void OnUpdate(float dt) { }
        void SetID(uint32_t id) { m_ID = id; }

    public:

        void SetWorld(const Ref<World>& world) { m_World = world; }

        GameObject(entt::entity entityID): m_ID(static_cast<uint32_t>(entityID)) {  }

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
        bool HasComponent()
        {
          
            auto& registry = m_World->GetRegistry();
            
            return registry.any_of<Component>(static_cast<entt::entity>(m_ID));;
        }

    };




    template <class  Component>
    Component& GameObject::GetComponent()
    {
    
        auto& registry = m_World->GetRegistry();

        auto view = registry.view<Component>();
        
        return view.get<Component>(static_cast<const entt::entity>(m_ID));
    
    }

    template <typename Component, typename ... Args>
    void GameObject::AddComponent(Args&&... args)
    {
       
	    if (HasComponent<Component>())
	    {
            spdlog::error("Component Already exist");
		    return;
	    }

        auto& registry = m_World->GetRegistry();
        registry.emplace<Component>(static_cast<const entt::entity>(m_ID), std::forward<Args>(args) ...);
    }

    template <typename Component, typename ... Args>
    void GameObject::AddOrReplaceComponent(Args&&... args)
    {
      
        if (HasComponent<Component>())
        {
            spdlog::error("Component Already exist");
            return;
        }

        auto& registry = m_World->GetRegistry();
        registry.emplace_or_replace<Component>(static_cast<const entt::entity>(m_ID), std::forward<Args>(args) ...);
    }

    template <typename Component>
    bool GameObject::RemoveComponent()
    {
      
        auto& registry = m_World->GetRegistry();

    	registry.remove<Component>(static_cast<const entt::entity>(m_ID));
      
        return false;
    }




}
