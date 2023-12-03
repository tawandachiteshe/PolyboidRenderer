#pragma once
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Engine/Engine/UUID.h"


namespace Polyboid
{
	struct TagComponent
	{
		std::string name;

		TagComponent() = default;
		TagComponent(std::string name): name(std::move(name)){}
	};

	struct UUIDComponent
	{
		UUID id;

		UUIDComponent() = default;
		UUIDComponent(const UUID& _id) : id(_id) {}

	};

    struct TransformComponent
    {
        glm::vec3 Position = glm::vec3(0.f);
        glm::vec3 Scale = glm::vec3(1.f);
        glm::vec3 Rotation = glm::vec3(0.f);

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position)
            : Position(position) {}

        [[nodiscard]]
        glm::mat4 GetTransform() const
        {
            const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }

        [[nodiscard]]
        glm::mat4 GetTransform()
        {
            const glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));

            return glm::translate(glm::mat4(1.0f), Position)
                * rotation
                * glm::scale(glm::mat4(1.0f), Scale);
        }

    };

    enum class ShapeType
    {
	    QUAD
    };


    struct ShapeComponent
    {

        ShapeType Type = ShapeType::QUAD;
        glm::vec4 Color{1.0f};

    };


}
