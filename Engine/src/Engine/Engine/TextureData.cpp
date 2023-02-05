#include "boidpch.h"
#include "TextureData.h"

#include <assimp/texture.h>
#include <spdlog/spdlog.h>
#include <stb/stb_image.h>


namespace Polyboid
{
	TextureData::TextureData(const aiTexture* texture)
	{
		m_Data = stbi_load_from_memory((stbi_uc const*)texture->pcData, static_cast<int>(texture->mWidth), &m_Width,
			&m_Height, &m_Channels, 0);
	}


	TextureData::~TextureData()
	{
		
		if (m_Width != 1)
		{
			stbi_image_free(m_Data);
		}
		
	}
}
