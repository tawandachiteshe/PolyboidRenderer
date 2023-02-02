#include "boidpch.h"
#include "FrustumCulling.h"

#include "Camera.h"


namespace Polyboid
{
	bool FrustumCulling::IsBoxInFrustum(const AABB& b)
	{
		using glm::dot;
		using glm::vec4;
		for (int i = 0; i < 6; i++)
		{
			int r = 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMin().x, b.GetMin().y, b.GetMin().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMax().x, b.GetMin().y, b.GetMin().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMin().x, b.GetMax().y, b.GetMin().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMax().x, b.GetMax().y, b.GetMin().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMin().x, b.GetMin().y, b.GetMax().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMax().x, b.GetMin().y, b.GetMax().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMin().x, b.GetMax().y, b.GetMax().z, 1.f)) < 0)
				? 1
				: 0;
			r += (dot(frPlanes[i],
				vec4(b.GetMax().x, b.GetMax().y, b.GetMax().z, 1.f)) < 0)
				? 1
				: 0;
			if (r == 8) return false;
		}


		int r = 0;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].x > b.GetMax().x) ? 1 : 0);
		if (r == 8) return false;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].x < b.GetMin().x) ? 1 : 0);
		if (r == 8) return false;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].y > b.GetMax().y) ? 1 : 0);
		if (r == 8) return false;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].y < b.GetMin().y) ? 1 : 0);
		if (r == 8) return false;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].z > b.GetMax().z) ? 1 : 0);
		if (r == 8) return false;
		r = 0;
		for (int i = 0; i < 8; i++)
			r += ((frCorners[i].z < b.GetMin().z) ? 1 : 0);
		if (r == 8) return false;
		return true;
	}

	FrustumCulling::FrustumCulling(const Ref<Camera>& camera)
	{
		GetFrustumPlanes(camera->GetViewProjection(), frPlanes);
		GetFrustumCorners(camera->GetViewProjection(), frCorners);
	}

	void FrustumCulling::GetFrustumCorners(const glm::mat4& vp, std::array<glm::vec4, 8>& points)
	{
		using glm::vec4;
		const vec4 corners[] = {
			vec4(-1, -1, -1, 1), vec4(1, -1, -1, 1),
			vec4(1, 1, -1, 1), vec4(-1, 1, -1, 1),
			vec4(-1, -1, 1, 1), vec4(1, -1, 1, 1),
			vec4(1, 1, 1, 1), vec4(-1, 1, 1, 1)
		};
		const glm::mat4 invVP = glm::inverse(vp);
		for (int i = 0; i != 8; i++)
		{
			const vec4 q = invVP * corners[i];
			points[i] = q / q.w;
		}
	}

	void FrustumCulling::GetFrustumPlanes(const glm::mat4& _vp, std::array<glm::vec4, 6>& planes)
	{
		using glm::vec4;
		glm::mat4 vp = glm::transpose(_vp);
		planes[0] = vec4(vp[3] + vp[0]); // left
		planes[1] = vec4(vp[3] - vp[0]); // right
		planes[2] = vec4(vp[3] + vp[1]); // bottom
		planes[3] = vec4(vp[3] - vp[1]); // top
		planes[4] = vec4(vp[3] + vp[2]); // near
		planes[5] = vec4(vp[3] - vp[2]); // far
	}
}
