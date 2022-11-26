#pragma once

#include <string>

#include "Engine/Engine/UUID.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"
#include "glm/gtx/transform.hpp"
#include "Engine/Engine/Base.h"

namespace Polyboid
{
	enum class ShapeType  
	{
		Quad,
        Circle
	};

    enum class CameraProjectionType
    {
        Orthographic, 
        Perspective
    };

    //Example component maybe might need
	struct MovementComponent
	{
        glm::vec3 direction;

        MovementComponent() = default;
        MovementComponent(const MovementComponent&) = default;
        MovementComponent(const glm::vec3& direction) : direction(direction) {}
	};

	struct IDComponent
	{
        UUID id;

        IDComponent() = default;
        IDComponent(const IDComponent&) = default;
        IDComponent(const UUID& id): id(id) {}


	};

    class Camera;


    //thinking about this...
    struct CameraComponent
    {

        CameraProjectionType Type;
        Ref<Camera> Camera;

        bool Current = false;

        float FOV = 30.0f;
        float Near = 0.01f;
        float Far = 1000.0f;
    	float Size = 10.0f;


    	CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;


    };

    struct TransformComponent
    {
        glm::vec3 Position  = glm::vec3(0.f);
        glm::vec3 Scale     = glm::vec3(1.f);
        glm::vec3 Rotation  = glm::vec3(0.f);

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
        
    };


    struct ShapeComponent
    {
        ShapeType type = ShapeType::Quad;
        glm::vec4 color = glm::vec4{ 1.0f };
        float thickness = 0.0f;
        float fade = 0.0f;

        ShapeComponent() = default;
        ShapeComponent(const ShapeComponent& shape) = default;
        ShapeComponent(const glm::vec4& color): color(color) {}
    };

    struct TagComponent
    {
        std::string name;
    };
}
