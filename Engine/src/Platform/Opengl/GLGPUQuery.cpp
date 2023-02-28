#include "boidpch.h"
#include "GLGPUQuery.h"

namespace Polyboid
{
	//GL_GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX

	GLenum ConvertQueryTypeToGL(QueryType queryType) {
		switch (queryType) {
			
		case QueryType::SamplesPassed:
			return GL_SAMPLES_PASSED;
		case QueryType::AnySamplesPassed:
			return GL_ANY_SAMPLES_PASSED;
		case QueryType::PrimitivesGenerated:
			return GL_PRIMITIVES_GENERATED;
		case QueryType::TransformFeedbackPrimitivesWritten:
			return GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN;
		case QueryType::TimeElapsed:
			return GL_TIME_ELAPSED;
		case QueryType::Timestamp:
			return GL_TIMESTAMP;
		default:
			// Return an invalid value if an unknown query type is passed in.
			return 0;
		}
	}

	

	GLGPUQuery::GLGPUQuery(QueryType t_query)
	{
		m_Query = ConvertQueryTypeToGL(t_query);
		glCreateQueries(m_Query, 1, &m_Handle);
	}

	uint64_t GLGPUQuery::GetResult()
	{
		GLuint64 result;
		glGetQueryObjectui64v(m_Handle, GL_QUERY_RESULT, &result);

		return result;
	}

	void GLGPUQuery::Begin()
	{
		glBeginQuery(m_Query, m_Handle);
	}

	void GLGPUQuery::End()
	{
		glEndQuery(m_Query);
	}

	GLGPUQuery::~GLGPUQuery()
	{
		
		glDeleteQueries(1, &m_Handle);
	}
}
