#pragma once

#include <string>

#include "Engine/Engine/UUID.h"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_normalized_axis.hpp"
#include "glm/gtx/transform.hpp"
#include "Engine/Engine/Base.h"
#include "spdlog/spdlog.h"

namespace Polyboid
{
	enum class ShapeType  
	{
		Quad,
        Circle
	};

    enum class LightType
    {
        Directional = 0,
        Spot = 1,
        Point = 2
	};

    enum class AttentuationType
    {
        InverseSquare = 0,
    	Exp = 1,
        Smooth = 2
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


    struct LightComponent
    {
        glm::vec4 color = glm::vec4{ 1.0f };
        LightType type = LightType::Directional;

        LightComponent(const glm::vec4& color) : color(color) {}
        LightComponent() = default;
        LightComponent(const LightComponent& shape) = default;

    };

    struct DirectionLightComponent : LightComponent
    {
        glm::vec3 direction = { 0, 1, 0};
        float Energy = 2.0;

        DirectionLightComponent(const glm::vec4& _color) : LightComponent(_color) {}
        DirectionLightComponent() = default;
        DirectionLightComponent(const DirectionLightComponent& shape) = default;
    };

    struct SpotLightComponent : LightComponent
    {
        glm::vec3 direction = { 0, 1, 0 };
        float InnerAngle = 45.0f;
        float OuterAngle = 2.5f;
        float Distance = 1.5;
        float Energy = 2.0;

        SpotLightComponent(const glm::vec4& _color) : LightComponent(_color) {}
        SpotLightComponent() = default;
        SpotLightComponent(const SpotLightComponent& shape) = default;

    };


    struct PointLightComponent : LightComponent
    {

        float Distance = 1.5;
        float Energy = 2.0;
        

        PointLightComponent(const glm::vec4& _color) : LightComponent(_color) {}
        PointLightComponent() = default;
        PointLightComponent(const PointLightComponent& shape) = default;



    };

    struct MeshRendererComponent
    {
        std::string assetName;
        UUID assetId;
        uint64_t materialId = assetId;

        MeshRendererComponent() = default;
        MeshRendererComponent(const MeshRendererComponent& mesh) = default;
        explicit MeshRendererComponent(const UUID& assetID) : assetId(assetID) {}
        explicit MeshRendererComponent(std::string assetName) : assetName(std::move(assetName)) {}
    };

    struct TagComponent
    {
        std::string name;
    };
}
