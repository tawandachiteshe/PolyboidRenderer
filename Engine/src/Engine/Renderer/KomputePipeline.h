#pragma once
#include <any>

#include "RenderResource.h"
#include "Engine/Engine/Base.h"


namespace Polyboid
{
	class PipelineDescriptorSet;
	class UniformBufferSet;
	class Texture;
	class StorageBufferSet;
	class Shader;

	class KomputePipeline : public RenderResource
	{
	public:
		virtual void Bake() = 0;
		virtual void SetComputeShader(const Ref<Shader>& computeShader) = 0;
		virtual  std::vector<Ref<PipelineDescriptorSet>> AllocateDescriptorSets(uint32_t setBinding) = 0;
		virtual std::any GetHandle() = 0;

		virtual  std::vector<Ref<PipelineDescriptorSet>> GetDescriptorSets(uint32_t set)  = 0;

		virtual void BindUniformBufferSet(uint32_t binding, const Ref<UniformBufferSet>& bufferSet,
			uint32_t setBinding) = 0;
		virtual  void BindStorageBufferSet(uint32_t binding, const Ref<StorageBufferSet>& bufferSet,
			uint32_t setBinding) = 0;
		virtual void BindTexture2D(uint32_t binding, const Ref<Texture>& bufferSet, uint32_t setBinding) = 0;

		virtual void WriteSetResourceBindings(uint32_t set) = 0;

		static Ref<KomputePipeline> Create();
	};

}

