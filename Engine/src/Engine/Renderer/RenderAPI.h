#pragma once


#pragma once
#include <cstdint>

#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{

	class RenderAPI
	{
	public:

		static void Init()
		{
			glEnable(GL_BLEND);
			glBlendEquation(GL_FUNC_ADD);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
			glCullFace(GL_BACK);
			glFrontFace(GL_CW);
		}

		static void CreateViewport(const glm::vec2& viewportSize)
		{
			glViewport(0, 0, static_cast<GLsizei>(viewportSize.x), static_cast<GLsizei>(viewportSize.y));
		}

		static void Clear(const glm::vec4& color)
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(color.x, color.y, color.z, color.w);
		}

		static void DrawIndexed(uint32_t count, uint16_t elementCount = 2)
		{
			glDrawElements(GL_TRIANGLES, count, elementCount == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, nullptr);
		}
	};

}
