#include "boidpch.h"
#include "AABB.h"

#include <spdlog/spdlog.h>

#include "glm/gtx/component_wise.hpp"
#include "glm/gtx/extend.hpp"


namespace Polyboid
{
    AABB::AABB()
    {
        SetNull();
    }

    AABB::AABB(const glm::vec3& center, glm::f32 radius)
    {
        SetNull();
        Extend(center, radius);
    }

    AABB::AABB(const glm::vec3& p1, const glm::vec3& p2)
    {
        SetNull();
        Extend(p1);
        Extend(p2);
    }

    AABB::AABB(const AABB& aabb)
    {
        SetNull();
        Extend(aabb);
    }

    AABB::~AABB()
    {
        //spdlog::info("AABB destroyed");
    }

    void AABB::Extend(glm::f32 val)
    {
        if (!IsNull())
        {
            mMin -= glm::vec3(val);
            mMax += glm::vec3(val);
        }
    }

    void AABB::Extend(const glm::vec3& p)
    {
        if (!IsNull())
        {
            mMin = glm::min(p, mMin);
            mMax = glm::max(p, mMax);
        }
        else
        {
            mMin = p;
            mMax = p;
        }
    }

    void AABB::Extend(const glm::vec3& p, glm::f32 radius)
    {
        glm::vec3 r(radius);
        if (!IsNull())
        {
            mMin = glm::min(p - r, mMin);
            mMax = glm::max(p + r, mMax);
        }
        else
        {
            mMin = p - r;
            mMax = p + r;
        }
    }

    void AABB::Extend(const AABB& aabb)
    {
        if (!aabb.IsNull())
        {
            Extend(aabb.mMin);
            Extend(aabb.mMax);
        }
    }

    void AABB::Merge(const AABB& aabb)
    {
        mMin.x = glm::min(mMin.x, aabb.mMin.x);
    	mMin.y = glm::min(mMin.y, aabb.mMin.y);
    	mMin.z = glm::min(mMin.z, aabb.mMin.z);
    	mMax.x = glm::max(mMax.x, aabb.mMax.x);
        mMax.y = glm::max(mMax.y, aabb.mMax.y);
    	mMax.z = glm::max(mMax.z, aabb.mMax.z);
    }

    void AABB::Add(const AABB& aabb)
    {
        mMin += aabb.mMin;
        mMax += aabb.mMax;
    }

    void AABB::ExtendDisk(const glm::vec3& c, const glm::vec3& n, glm::f32 r)
    {
        if (glm::length(n) < 1.e-12f) { Extend(c); return; }
        glm::vec3 norm = glm::normalize(n);
        glm::f32 x = sqrt(1 - norm.x) * r;
        glm::f32 y = sqrt(1 - norm.y) * r;
        glm::f32 z = sqrt(1 - norm.z) * r;
        Extend(c + glm::vec3(x, y, z));
        Extend(c - glm::vec3(x, y, z));
    }

    glm::vec3 AABB::GetDiagonal() const
    {
        if (!IsNull())
            return mMax - mMin;
        else
            return glm::vec3(0);
    }

    glm::f32 AABB::GetLongestEdge() const
    {

        return glm::compMax(GetDiagonal());
    }

    glm::f32 AABB::GetShortestEdge() const
    {
        return glm::compMin(GetDiagonal());
    }

    glm::vec3 AABB::GetCenter() const
    {
        if (!IsNull())
        {
            glm::vec3 d = GetDiagonal();
            return mMin + (d * glm::f32(0.5f));
        }
        else
        {
            return glm::vec3(0.0);
        }
    }

    void AABB::Translate(const glm::vec3& v)
    {
        if (!IsNull())
        {
            mMin += v;
            mMax += v;
        }
    }

    void AABB::Transform(const glm::mat4& mat)
    {
        if (!IsNull())
        {
            mMin += glm::vec3(mat * glm::vec4(mMin, 1.0f));
            mMax += glm::vec3(mat * glm::vec4(mMax, 1.0f));
        }
    }

    void AABB::Scale(const glm::vec3& s, const glm::vec3& o)
    {
        if (!IsNull())
        {
            mMin -= o;
            mMax -= o;

            mMin *= s;
            mMax *= s;

            mMin += o;
            mMax += o;
        }
    }

    bool AABB::Overlaps(const AABB& bb) const
    {
        if (IsNull() || bb.IsNull())
            return false;

        if (bb.mMin.x > mMax.x || bb.mMax.x < mMin.x)
            return false;
        else if (bb.mMin.y > mMax.y || bb.mMax.y < mMin.y)
            return false;
        else if (bb.mMin.z > mMax.z || bb.mMax.z < mMin.z)
            return false;

        return true;
    }

    AABB::INTERSECTION_TYPE AABB::Intersect(const AABB& b) const
    {
        if (IsNull() || b.IsNull())
            return OUTSIDE;

        if ((mMax.x < b.mMin.x) || (mMin.x > b.mMax.x) ||
            (mMax.y < b.mMin.y) || (mMin.y > b.mMax.y) ||
            (mMax.z < b.mMin.z) || (mMin.z > b.mMax.z))
        {
            return OUTSIDE;
        }

        if ((mMin.x <= b.mMin.x) && (mMax.x >= b.mMax.x) &&
            (mMin.y <= b.mMin.y) && (mMax.y >= b.mMax.y) &&
            (mMin.z <= b.mMin.z) && (mMax.z >= b.mMax.z))
        {
            return INSIDE;
        }

        return INTERSECT;
    }


    bool AABB::IsSimilarTo(const AABB& b, glm::f32 diff) const
    {
        if (IsNull() || b.IsNull()) return false;

        glm::vec3 acceptable_diff = ((GetDiagonal() + b.GetDiagonal()) / glm::f32(2.0f)) * diff;
        glm::vec3 min_diff(mMin - b.mMin);
        min_diff = glm::vec3(fabs(min_diff.x), fabs(min_diff.y), fabs(min_diff.z));
        if (min_diff.x > acceptable_diff.x) return false;
        if (min_diff.y > acceptable_diff.y) return false;
        if (min_diff.z > acceptable_diff.z) return false;
        glm::vec3 max_diff(mMax - b.mMax);
        max_diff = glm::vec3(fabs(max_diff.x), fabs(max_diff.y), fabs(max_diff.z));
        if (max_diff.x > acceptable_diff.x) return false;
        if (max_diff.y > acceptable_diff.y) return false;
        if (max_diff.z > acceptable_diff.z) return false;
        return true;
    }
}
