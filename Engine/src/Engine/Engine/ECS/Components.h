#pragma once

#include <string>

#include "glm/fwd.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"
#include "glm/gtx/transform.hpp"

namespace Polyboid
{
	enum class ShapeType  
	{
		Quad
	};

    struct Transform
    {
        glm::vec3 Position = glm::vec3(0.f);
        glm::vec3 Scale = glm::vec3(1.f);
        glm::vec3 Rotation = glm::vec3(0.f);

        Transform() = default;
        Transform(const Transform&) = default;
        Transform(const glm::vec3& position)
            : Position(position) {}

        glm::mat4 GetTransform() const
        {
            const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }
        
    };


    struct Shape
    {
        ShapeType type = ShapeType::Quad;
        glm::vec4 color = glm::vec4{ 1.0f };
        Shape() = default;
        Shape(const Shape& shape) = default;
        Shape(const glm::vec4& color): color(color) {}
    };

    struct Tag
    {
        std::string name;
    };
}
