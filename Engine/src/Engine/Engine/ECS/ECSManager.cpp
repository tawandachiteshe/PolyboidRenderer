#include "boidpch.h"
#include "ECSManager.h"

namespace Polyboid
{

    std::unique_ptr<ECSManagerData> ECSManager::s_Data = nullptr;

    void ECSManager::Init()
    {
        s_Data = std::make_unique<ECSManagerData>();
    }

    entt::registry& ECSManager::GetRegistry()
    {
        return s_Data->m_Registry;
    }

    uint32_t ECSManager::CreateEntity()
    {
        return (uint32_t)s_Data->m_Registry.create();
    }
}
