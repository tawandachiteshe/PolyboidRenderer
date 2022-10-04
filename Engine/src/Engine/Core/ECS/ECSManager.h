#pragma once
#include "entt/entt.hpp"

namespace Polyboid
{
    struct ECSManagerData
    {
        entt::registry m_Registry;
    };
    class ECSManager
    {
    private:

        static std::unique_ptr<ECSManagerData> s_Data;
        
    public:
        static void Init();
        static entt::registry& GetRegistry();
        static uint32_t CreateEntity();
    };

}

