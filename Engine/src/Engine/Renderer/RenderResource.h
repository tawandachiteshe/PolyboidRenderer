#pragma once

namespace Polyboid
{

    enum class RenderResourceType
    {
        Image,
        TexelStorageBuffer,
        TexelUniformBuffer,
        Texture2D,
        Texture3D,
        StorageBuffer,
        UniformBuffer,
        None
    };

	class RenderResource
	{
	public:
		virtual void Destroy() = 0;
		virtual void Recreate() = 0;
		virtual ~RenderResource() {}
        virtual  RenderResourceType GetRenderResourceType() = 0;
	};

}

