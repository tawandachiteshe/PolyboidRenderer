#pragma once
#include "glad/glad.h"

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

		static void DrawIndexed(uint32_t count)
		{
			glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, nullptr);
		}
	};

}
