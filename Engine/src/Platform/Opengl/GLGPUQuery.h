#pragma once
#include "Engine/Renderer/GPUQuery.h"


namespace Polyboid
{
	class GLGPUQuery : public GPUQuery
	{
	private:
		uint32_t m_Handle = 0;
		GLenum m_Query = 0;
	public:
		GLGPUQuery(QueryType type);
		uint64_t GetResult() override;
		void Begin() override;
		void End() override;
		~GLGPUQuery() override;
	};
}
