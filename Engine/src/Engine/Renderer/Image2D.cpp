#include "boidpch.h"
#include "Image2D.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<Image2D> Image2D::Create(const ImageSettings& settings)
	{

		return RenderAPI::Get()->CreateImage2D(settings);

	}
}
