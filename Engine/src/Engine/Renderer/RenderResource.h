#pragma once

namespace Polyboid
{
	class RenderResource
	{
	public:
		virtual void Destroy() = 0;
		virtual void Recreate() = 0;
		virtual ~RenderResource() {}
	};

}

