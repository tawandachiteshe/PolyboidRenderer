#pragma once

#include <string>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"
#include "glm/gtx/transform.hpp"

namespace Polyboid
{
    struct Transform
    {
        glm::vec3 Position = glm::vec3(0.f);
        glm::vec3 Scale = glm::vec3(1.f);
        glm::vec3 Rotation = glm::vec3(0.f);
    };

    struct Tag
    {
        std::string name;
    };
}
