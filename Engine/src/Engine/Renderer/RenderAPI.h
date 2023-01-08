#pragma once


#pragma once
#include <cstdint>

#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
	enum class ClearMode { Depth, Color, DepthColor };

	class RenderAPI
	{
	public:

		static void Init()
		{
			POLYBOID_PROFILE_FUNCTION();

			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

		}

		static void CullFront()
		{
			glCullFace(GL_FRONT);
		}

		static void CullBack()
		{
			glCullFace(GL_BACK);
		}


		static void CullFrontBack()
		{
			glCullFace(GL_FRONT_AND_BACK);
		}

		static void CullNone()
		{
			glCullFace(GL_NONE);
		}


		static void CreateViewport(const glm::vec2& viewportSize)
		{
			POLYBOID_PROFILE_FUNCTION();

			glViewport(0, 0, static_cast<GLsizei>(viewportSize.x), static_cast<GLsizei>(viewportSize.y));
		}

		static void SetClearColor(const glm::vec4& color)
		{
			glClearColor(color.x, color.y, color.z, color.w);
		}

		static void Clear(const ClearMode& mode)
		{
			POLYBOID_PROFILE_FUNCTION();

			switch (mode)
			{
			case ClearMode::Color:
				glClear(GL_COLOR_BUFFER_BIT);
				break;
			case ClearMode::Depth:
				glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				break;
			case ClearMode::DepthColor:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				break;
			}

		}

		static void DrawIndexed(uint32_t count, uint16_t elementCount = 2)
		{
			POLYBOID_PROFILE_FUNCTION();

			glDrawElements(GL_TRIANGLES, count, elementCount == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, nullptr);
		}
	};

}
