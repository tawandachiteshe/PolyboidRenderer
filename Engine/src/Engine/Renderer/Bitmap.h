#pragma once
#include <string>

#include "glm/vec4.hpp"

namespace Polyboid
{
	class Bitmap
	{

	private:
		int32_t m_Width;
		int32_t m_Height;
		int32_t m_Channels;
		float* m_Pixels = nullptr;
		bool stbImage = false;

	public:

		static void WriteBitmapToFile(const Bitmap& bitmap, const std::string& path);
		Bitmap(const Bitmap& map): m_Width(map.m_Width), m_Height(map.m_Height), m_Channels(map.m_Channels), m_Pixels(map.m_Pixels) { }
		Bitmap() = default;
		Bitmap(const std::string& path);
		//Bitmap(int32_t width, int32_t height, int32_t channels);
		Bitmap(int32_t width, int32_t height, int32_t channels, int32_t numOfbitMaps = 1);
		Bitmap(int32_t width, int32_t height, int32_t channels, float* data);

		glm::vec4 ReadPixel(int32_t x, int32_t y) const;
		void WritePixel(int32_t x, int32_t y, const glm::vec4& color);

		[[nodiscard]] float* GetPixels() const { return m_Pixels; }
		[[nodiscard]] int32_t GetWidth() const { return m_Width; }
		[[nodiscard]] int32_t GetHeight() const { return m_Height; }
		[[nodiscard]] int32_t GetChannels() const { return m_Channels; }

		~Bitmap();

	};
}
