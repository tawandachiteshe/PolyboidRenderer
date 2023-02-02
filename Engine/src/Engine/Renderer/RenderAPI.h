#pragma once


#pragma once
#include <cstdint>

#include "Engine/Engine/Debug/Profiler.h"
#include "glad/glad.h"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

namespace Polyboid
{
	enum class ClearMode { Depth, Color, Stencil, DepthColor, DepthColorStencil };

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
			glEnable(GL_STENCIL_TEST);
			glCullFace(GL_BACK);
			glFrontFace(GL_CCW);
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		}

		static void SetDepthState()
		{
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_GREATER);
		}

		static void ResetDepthState()
		{
			glDepthMask(GL_TRUE);
			glDepthFunc(GL_LESS);
		}


		static void SetStencilState()
		{
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
		}

		static void ResetStencilState()
		{
			glStencilFunc(GL_ALWAYS, 0, 1);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
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

		static void SetStencilValue(int32_t value)
		{
			glClearStencil(value);
		}

		static void SetDepthValue(double value)
		{
			glClearDepth(value);
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
				glClear(GL_DEPTH_BUFFER_BIT);
				break;
			case ClearMode::Stencil:
				glClear(GL_STENCIL_BUFFER_BIT);
				break;
			case ClearMode::DepthColor:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				break;
			case ClearMode::DepthColorStencil:
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
				break;
			}

		}

		static void DrawLines(uint32_t vertexCount)
		{
			glDrawArrays(GL_LINES, 0, vertexCount);
		}

		static void DrawIndexed(uint32_t count, uint16_t elementCount = 2)
		{
			POLYBOID_PROFILE_FUNCTION();

			glDrawElements(GL_TRIANGLES, count, elementCount == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, nullptr);
		}
	};

}
