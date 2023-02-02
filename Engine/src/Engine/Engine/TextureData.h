#pragma once


struct aiTexture;

namespace Polyboid
{
	class TextureData
	{
	private:
		int32_t m_Width = 0;
		int32_t m_Height = 0;
		int32_t m_Channels = 0;

		void* m_Data = nullptr;


	public:
		TextureData(int32_t m_width, int32_t m_height, int32_t m_channels, void* m_data)
			: m_Width(m_width),
			  m_Height(m_height),
			  m_Channels(m_channels),
			  m_Data(m_data)
		{
		}

		TextureData() = default;
		TextureData(const aiTexture* texture);
		TextureData(const TextureData& data) = default;
		~TextureData();

		[[nodiscard]] int32_t GetWidth() const
		{
			return m_Width;
		}

		[[nodiscard]] int32_t GetHeight() const
		{
			return m_Height;
		}

		[[nodiscard]] int32_t GetChannels() const
		{
			return m_Channels;
		}

		[[nodiscard]] void* GetData() const
		{
			return m_Data;
		}
	};
}
