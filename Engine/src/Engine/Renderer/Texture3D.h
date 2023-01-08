#pragma once


namespace Polyboid
{
	class Texture3D
	{
	private:
		uint32_t m_ID = 0;
		uint32_t m_Sampler = 0;
		uint32_t m_Resolution = 0;

	public:

		uint32_t GetID() { return m_ID; }

		Texture3D() = default;
		Texture3D(const std::string& path);
		Texture3D(void** data, int32_t resolution, int32_t channels = 3);

		//Old implentation previous open gl version needs internet :)

		Texture3D(uint32_t resolution, uint32_t mipLevels = 1);

		void Bind(uint32_t textureUnit = 0);
	};
}
