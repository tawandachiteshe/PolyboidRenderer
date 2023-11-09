#pragma once
#include <any>


namespace Polyboid
{
	struct ImageViewSettings
	{

		uint32_t MipLevels = 1;
		uint32_t LayerCount = 1;
	};

	class ImageView2D
	{
	public:
		virtual std::any GetHandle() = 0;
		virtual ~ImageView2D() {}
	};


	class ImageView3D
	{
	public:
		virtual std::any GetHandle() = 0;
		virtual ~ImageView3D() {}
	};

}

