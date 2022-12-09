#pragma once


namespace Polyboid
{
	class Texture3D
	{
	private:
		uint32_t m_ID = 0;
	public:
		Texture3D() = default;
		Texture3D(const std::string& path);
		void Bind();
	};
}
