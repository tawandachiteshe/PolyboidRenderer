#pragma once


namespace Polyboid
{
	class MultiSampleState
	{
	public:

		virtual void SetSampleMask(uint32_t sampleMask) = 0;
		virtual uint32_t GetSampleMask() const = 0;

		virtual void SetAlphaCoverage(bool enabled) = 0;
		virtual bool GetAlphaCoverage() const = 0;

		virtual void SetSamplesCount(uint32_t count) = 0;

		virtual ~MultiSampleState() = default;



	};

}

