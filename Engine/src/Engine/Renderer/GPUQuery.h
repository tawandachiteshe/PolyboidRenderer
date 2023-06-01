#pragma once

namespace Polyboid
{


// #define GL_VERTICES_SUBMITTED 0x82EE
// #define GL_PRIMITIVES_SUBMITTED 0x82EF
// #define GL_VERTEX_SHADER_INVOCATIONS 0x82F0
// #define GL_TESS_CONTROL_SHADER_PATCHES 0x82F1
// #define GL_TESS_EVALUATION_SHADER_INVOCATIONS 0x82F2
// #define GL_GEOMETRY_SHADER_PRIMITIVES_EMITTED 0x82F3
// #define GL_FRAGMENT_SHADER_INVOCATIONS 0x82F4
// #define GL_COMPUTE_SHADER_INVOCATIONS 0x82F5
// #define GL_CLIPPING_INPUT_PRIMITIVES 0x82F6
// #define GL_CLIPPING_OUTPUT_PRIMITIVES 0x82F7

enum class QueryType
{
	SamplesPassed,
	AnySamplesPassed,
	PrimitivesGenerated,
	TransformFeedbackPrimitivesWritten,
	TimeElapsed,
	Timestamp
};

	class GPUQuery
	{
	public:
		virtual uint64_t GetResult() = 0;
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual ~GPUQuery() = default;
	};
}

