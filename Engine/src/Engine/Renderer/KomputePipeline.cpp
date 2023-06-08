#include "boidpch.h"
#include "KomputePipeline.h"

#include "RenderAPI.h"


namespace Polyboid
{
	Ref<KomputePipeline> KomputePipeline::Create()
	{
		return RenderAPI::Get()->CreateKomputePipeline();
	}
}
