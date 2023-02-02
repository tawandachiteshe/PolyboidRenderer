#pragma once
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Math/AABB.h"
#include "glm/vec4.hpp"


namespace Polyboid
{
	class Camera;

	class FrustumCulling
	{
	public:
		bool IsBoxInFrustum(const AABB& b);
		FrustumCulling(const Ref<Camera>& camera);
		static void GetFrustumCorners(const glm::mat4& vp, std::array<glm::vec4, 8>& points);
		static void GetFrustumPlanes(const glm::mat4& vp, std::array<glm::vec4, 6>& planes);

	private:

		std::array<glm::vec4, 6> frPlanes = { glm::vec4{0} };
		std::array<glm::vec4, 8> frCorners = {  glm::vec4{0} };
	};
	
}
