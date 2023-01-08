#pragma once
#include  <glm/glm.hpp>
#include <stb/stb_image_resize.h>
#include <vector>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

#include "Engine/Engine/Debug/Timer.h"
#include "glm/ext/scalar_constants.hpp"
#include "glm/gtc/constants.hpp"


namespace Polyboid
{

	float radicalInverse_VdC(uint32_t bits) {
		bits = (bits << 16u) | (bits >> 16u);
		bits = ((bits & 0x55555555u) << 1u) |
			((bits & 0xAAAAAAAAu) >> 1u);
		bits = ((bits & 0x33333333u) << 2u) |
			((bits & 0xCCCCCCCCu) >> 2u);
		bits = ((bits & 0x0F0F0F0Fu) << 4u) |
			((bits & 0xF0F0F0F0u) >> 4u);

		bits = ((bits & 0x00FF00FFu) << 8u) |
			((bits & 0xFF00FF00u) >> 8u);

		return float(bits) * 2.3283064365386963e-10f;
	}

	glm::vec2 hammersley2d(uint32_t i, uint32_t N) {
		return glm::vec2(float(i) / float(N),
			radicalInverse_VdC(i));
	}

	static  void convolveDiffuse(const glm::vec3* data,
		int srcW, int srcH, int dstW, int dstH,
		glm::vec3* output, int numMonteCarloSamples)
	{
		assert(srcW == 2 * srcH);
		if (srcW != 2 * srcH) return;
		std::vector<glm::vec3> tmp(dstW * dstH);
		stbir_resize_float_generic(
			reinterpret_cast<const float*>(data), srcW, srcH,
			0,
			reinterpret_cast<float*>(tmp.data()), dstW, dstH,
			0, 3, STBIR_ALPHA_CHANNEL_NONE, 0,
			STBIR_EDGE_CLAMP, STBIR_FILTER_CUBICBSPLINE,
			STBIR_COLORSPACE_LINEAR, nullptr);
		const glm::vec3* scratch = tmp.data();
		srcW = dstW;
		srcH = dstH;

		for (int y = 0; y != dstH; y++)
		{
			const float theta1 =
				float(y) / float(dstH) * glm::pi<float>();
			for (int x = 0; x != dstW; x++)
			{
				const float phi1 =
					float(x) / float(dstW) * glm::two_pi<float>();
				const glm::vec3 V1 = glm::vec3(sin(theta1) * cos(phi1),
					sin(theta1) * sin(phi1), cos(theta1));
				glm::vec3 color = glm::vec3(0.0f);
				float weight = 0.0f;
				for (int i = 0; i != numMonteCarloSamples; i++)
				{
					const glm::vec2 h =
						hammersley2d(i, numMonteCarloSamples);
					const int x1 = int(floor(h.x * srcW));
					const int y1 = int(floor(h.y * srcH));
					const float theta2 =
						float(y1) / float(srcH) * glm::pi<float>();
					const float phi2 =
						float(x1) / float(srcW) * glm::two_pi<float>();
					const glm::vec3 V2 = glm::vec3(sin(theta2) * cos(phi2),
						sin(theta2) * sin(phi2), cos(theta2));
					const float NdotL =
						std::max(0.0f, glm::dot(V1, V2));

					if (NdotL > 0.01f) {
						color += scratch[y1 * srcW + x1] * NdotL;
						weight += NdotL;
					}

				}


				output[y * dstW + x] = color / weight;
			}
		}
	}

	static  void ToImage()
	{
		
		Timer t;
		int width, height, channels;

		float* data = stbi_loadf("Assets/HDRs/thatch_chapel_2k.hdr", &width, &height, &channels, 0);
		auto* dstData = new float[width * height * channels];

		convolveDiffuse((glm::vec3*)data, width, height, width, height, (glm::vec3*)dstData, 64);
		stbi_write_hdr("tawanda.hdr", width, height, channels, dstData);

		stbi_image_free(data);
		delete[] dstData;

		spdlog::info("Image convo Took {} ms", t.ElapsedMillis());
		
	}
}
