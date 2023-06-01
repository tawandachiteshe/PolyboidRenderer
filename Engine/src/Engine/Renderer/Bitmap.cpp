#include "boidpch.h"
#include "Bitmap.h"

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>

namespace Polyboid
{
	void Bitmap::WriteBitmapToFile(const Bitmap& bitmap, const std::string& path)
	{
		stbi_write_hdr(path.c_str(), bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetChannels(), reinterpret_cast<const float*>(bitmap.GetPixels()));
	}

	Bitmap::Bitmap(const std::string& path): stbImage(true)
	{
		m_Pixels = stbi_loadf(path.c_str(), &m_Width, &m_Height, &m_Channels, 0);

	}
	//
	// Bitmap::Bitmap(int32_t width, int32_t height, int32_t channels) : m_Width(width), m_Height(height), m_Channels(channels)
	// {
	// 	const auto size = width * height * channels;
	// 	m_Pixels = new float[size];
	// 	std::memset(m_Pixels, 0.0f, size);
	// }

	Bitmap::Bitmap(int32_t width, int32_t height, int32_t channels, int32_t numOfbitMaps): m_Width(width), m_Height(height), m_Channels(channels)
	{
		const auto size = width * height * channels * numOfbitMaps;
		m_Pixels = new float[size];
		std::memset(m_Pixels, 0, size);
	}

	Bitmap::Bitmap(int32_t width, int32_t height, int32_t channels, float* data): m_Width(width), m_Height(height), m_Channels(channels), m_Pixels(data)
	{
	}

	glm::vec4 Bitmap::ReadPixel(int32_t x, int32_t y) const
	{
		if (m_Pixels != nullptr)
		{

			const auto PIXEL_FORMULA = y * m_Width * m_Channels + x * m_Channels;

			if (m_Channels == 4)
			{

				//Column based
				const float red = m_Pixels[PIXEL_FORMULA];
				const float green = m_Pixels[PIXEL_FORMULA + 1];
				const float blue = m_Pixels[PIXEL_FORMULA + 2];
				const float alpha = m_Pixels[PIXEL_FORMULA + 3];


				return { red, green , blue, alpha };
			}
			else if (m_Channels == 3)
			{

				const float red = m_Pixels[PIXEL_FORMULA];
				const float green = m_Pixels[PIXEL_FORMULA + 1];
				const float blue = m_Pixels[PIXEL_FORMULA + 2];
				const float alpha = 1.0f;

				return { red , green, blue , alpha };
			}
			else
			{
				return {};
			}

		}

		return {};

	}

	void Bitmap::WritePixel(int32_t x, int32_t y, const glm::vec4& color)
	{
		const auto PIXEL_FORMULA = y * m_Width * m_Channels + x * m_Channels;

		if (m_Channels == 4)
		{

			//Column based
			m_Pixels[PIXEL_FORMULA] = color.r;
			m_Pixels[PIXEL_FORMULA + 1] = color.g;
			m_Pixels[PIXEL_FORMULA + 2] = color.b;
			m_Pixels[PIXEL_FORMULA + 3] = color.a;

		}
		else if (m_Channels == 3)
		{

			m_Pixels[PIXEL_FORMULA] = color.r;
			m_Pixels[PIXEL_FORMULA + 1] = color.g;
			m_Pixels[PIXEL_FORMULA + 2] = color.b;
			//m_Pixels[PIXEL_FORMULA + 3] = PIXEL_SIZE;

		}


	}

	Bitmap::~Bitmap()
	{
		if (m_Pixels != nullptr && stbImage)
		{
			stbi_image_free(m_Pixels);
		}

		if (!stbImage)
		{
			//delete[] m_Pixels;
		}
		
	}
}
