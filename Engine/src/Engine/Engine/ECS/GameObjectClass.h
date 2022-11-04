#pragma once

namespace Polyboid
{
    class World;

    template <typename Class>
    class GameObjectClass : public Class
    {
    public:
        GameObjectClass() : Class()
        {

        }

        void SetWorld(World* world)
        {
            m_World = world;
        }

    };
}
