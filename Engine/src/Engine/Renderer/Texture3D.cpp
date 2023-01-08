#include "boidpch.h"
#include "Texture3D.h"

#include <spdlog/spdlog.h>

#include "Bitmap.h"
#include "Texture2D.h"
#include "Engine/Engine/Base.h"
#include "Engine/Engine/Serializer.h"
#include "Engine/Engine/Debug/Timer.h"
#include "Engine/Engine/Utils/FileSystem.h"
#include "Engine/Engine/Utils/HDRtoCubemap.hpp"
#include "glm/common.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/ext/scalar_constants.hpp"

namespace Polyboid
{
	[[maybe_unused]] static glm::vec3 FaceCoordToCubeXYZ(int32_t i, int32_t j, uint32_t faceID, int32_t faceSize)
	{
		const float A = 2.0f * static_cast<float>(i) / static_cast<float>(faceSize);
		const float B = 2.0f * static_cast<float>(j) / static_cast<float>(faceSize);

		if (faceID == 0)
			return {-1.0f, A - 1.0f, B - 1.0f};
		if (faceID == 1)
			return {A - 1.0f, -1.0f, 1.0f - B};
		if (faceID == 2)
			return {1.0f, A - 1.0f, 1.0f - B};
		if (faceID == 3)
			return {1.0f - A, 1.0f, 1.0f - B};
		if (faceID == 4)
			return {B - 1.0f, A - 1.0f, 1.0f};
		if (faceID == 5)
			return {1.0f - B, A - 1.0f, -1.0f};
		return {};
	}

	static Bitmap ConvertToVCross(const Bitmap& cubeTexture, const std::string& path)
	{
		const int32_t faceSize = cubeTexture.GetWidth() / 4;

		//we can flip the cross to show we not joking
		const int w = faceSize * 3;
		const int h = faceSize * 4;

		Bitmap result(w, h, cubeTexture.GetChannels());

		const glm::ivec2 kFaceOffsets[] = {
			glm::ivec2(faceSize, faceSize * 3),
			glm::ivec2(0, faceSize),
			glm::ivec2(faceSize, faceSize),
			glm::ivec2(faceSize * 2, faceSize),
			glm::ivec2(faceSize, 0),
			glm::ivec2(faceSize, faceSize * 2)
		};

		const int clampW = cubeTexture.GetWidth() - 1;
		const int clampH = cubeTexture.GetHeight() - 1;

		for (int face = 0; face != 6; face++)
		{
			for (int i = 0; i != faceSize; i++)
			{
				for (int j = 0; j != faceSize; j++)
				{
					const glm::vec3 P = FaceCoordToCubeXYZ(
						i, j, face, faceSize);
					const float R = hypot(P.x, P.y);
					const float theta = atan2(P.y, P.x);
					const float phi = atan2(P.z, R);

					constexpr auto M_PI = glm::pi<float>();

					const float Uf = float(2.0f * faceSize *
						(theta + M_PI) / M_PI);
					const float Vf = float(2.0f * faceSize *
						(M_PI / 2.0f - phi) / M_PI);

					const int U1 =
						glm::clamp(int(glm::floor(Uf)), 0, clampW);
					const int V1 =
						glm::clamp(int(glm::floor(Vf)), 0, clampH);
					const int U2 = glm::clamp(U1 + 1, 0, clampW);
					const int V2 = glm::clamp(V1 + 1, 0, clampH);

					const float s = Uf - U1;
					const float t = Vf - V1;

					const glm::vec4 A = cubeTexture.ReadPixel(U1, V1);
					const glm::vec4 B = cubeTexture.ReadPixel(U2, V1);
					const glm::vec4 C = cubeTexture.ReadPixel(U1, V2);
					const glm::vec4 D = cubeTexture.ReadPixel(U2, V2);

					const glm::vec4 color = A * (1 - s) * (1 - t) +
						B * (s) * (1 - t) +
						C * (1 - s) * t + D * (s) * (t);

					result.WritePixel(
						i + kFaceOffsets[face].x, j +
						kFaceOffsets[face].y, color);
				}
			}
		}

		Bitmap::WriteBitmapToFile(result, path);

		return result;
	}

	Bitmap ConvertVCrossToCubeMapFaces(const Bitmap& vCross)
	{
		const int faceWidth = vCross.GetWidth() / 3;
		const int faceHeight = vCross.GetHeight() / 4;

		//We assuming float is a the default data structure
		Bitmap cubeMap(faceWidth, faceHeight, vCross.GetChannels(), 6);
		std::vector<Bitmap> maps;


		const float* src = vCross.GetPixels();
		float* dst = cubeMap.GetPixels();

		for (int face = 0; face != 6; ++face)
		{
			for (int j = 0; j != faceHeight; ++j)
			{
				for (int i = 0; i != faceWidth; ++i)
				{
					int x = 0;
					int y = 0;

					switch (face)
					{
					// GL_TEXTURE_CUBE_MAP_POSITIVE_X
					case 0: x = i;
						y = faceHeight + j;
						break;
					// GL_TEXTURE_CUBE_MAP_NEGATIVE_X
					case 1: x = 2 * faceWidth + i;
						y = 1 * faceHeight + j;
						break;
					// GL_TEXTURE_CUBE_MAP_POSITIVE_Y
					case 2: x = 2 * faceWidth - (i + 1);
						y = 1 * faceHeight - (j + 1);
						break;
					// GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
					case 3: x = 2 * faceWidth - (i + 1);
						y = 3 * faceHeight - (j + 1);
						break;
					// GL_TEXTURE_CUBE_MAP_POSITIVE_Z
					case 4: x = 2 * faceWidth - (i + 1);
						y = vCross.GetHeight() - (j + 1);
						break;
					// GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
					case 5: x = faceWidth + i;
						y = faceHeight + j;
						break;
					}

					const auto PIXEL_FORMULA = y * vCross.GetWidth() + x * 3;
					std::memcpy(dst, &src[PIXEL_FORMULA], 12);
					dst += 3;
				}
			}
		}


		return cubeMap;
	}


	Texture3D::Texture3D(const std::string& path)
	{

		const auto cachedImageExist = FileSystem::GenCacheFilename(path);

		Ref<CubeMapFace<uint8_t>> cubemapFace = nullptr;

		if (std::filesystem::exists(cachedImageExist))
		{
			CubeMapSerializer s;
			cubemapFace = s.Read(cachedImageExist);
		}
		else
		{
			HDRToCubemap<uint8_t> cubemap(path, 512);
			CubeMapSerializer s(cubemap.GetCubeMapFace());
			s.Write(cachedImageExist);
			cubemapFace = cubemap.GetCubeMapFace();
		}

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(
			m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(m_ID, 1, GL_RGB32F, cubemapFace->GetResolution(), cubemapFace->GetResolution());

		glCreateSamplers(1, &m_Sampler);

		for (int i = 0; i != 6; ++i)
		{
			if (cubemapFace->GetIsUsingVector())
			{
				glTextureSubImage3D(m_ID, 0, 0, 0, i, cubemapFace->GetResolution(), cubemapFace->GetResolution(), 1, GL_RGB, GL_UNSIGNED_BYTE, cubemapFace->GetArrayFaces()[i].data());
			}
			else
			{
				glTextureSubImage3D(m_ID, 0, 0, 0, i, cubemapFace->GetResolution(), cubemapFace->GetResolution(), 1, GL_RGB, GL_UNSIGNED_BYTE, cubemapFace->GetFaces()[i]);
			}
			
		}


	}

	Texture3D::Texture3D(void** data, int32_t resolution, int32_t channels)
	{

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(
			m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(m_ID, 1, GL_RGB32F, resolution, resolution);

		glCreateSamplers(1, &m_Sampler);


		for (int i = 0; i != 6; ++i)
		{
			
			glTextureSubImage3D(m_ID, 0, 0, 0, i, resolution, resolution, 1, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
		}
	}

	Texture3D::Texture3D(uint32_t resolution, uint32_t mipLevels): m_Resolution(resolution)
	{

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_ID);

		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTextureParameteri(
			m_ID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);

		if (mipLevels > 1)
		{
			glTextureParameteri(
				m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		}
		else
		{

			glTextureParameteri(
				m_ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}


		glTextureParameteri(
			m_ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureStorage2D(m_ID, mipLevels, GL_RGBA32F, static_cast<int>(resolution), static_cast<int>(resolution));

		if (mipLevels > 1)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		}

		glCreateSamplers(1, &m_Sampler);

		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}

	void Texture3D::Bind(uint32_t textureUnit)
	{
		glBindTextureUnit(textureUnit, m_ID);
		glBindSampler(textureUnit, m_Sampler);
	}
}
