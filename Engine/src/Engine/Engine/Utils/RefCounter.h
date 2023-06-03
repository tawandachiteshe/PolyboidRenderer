#pragma once
#include <cassert>
#include "Allocators.h"

namespace Polyboid
{

	class RefPtrCounter
	{

	private:
		std::atomic_int32_t* m_Count;

	public:

		RefPtrCounter(): m_Count(new std::atomic_int32_t(1)) {  }


		RefPtrCounter(const RefPtrCounter& other)
			: m_Count(other.m_Count)
		{
		}

		RefPtrCounter(RefPtrCounter&& other) noexcept
			: m_Count(other.m_Count)
		{
		}

		RefPtrCounter& operator=(const RefPtrCounter& other)
		{
			if (this == &other)
				return *this;
			m_Count = other.m_Count;
			return *this;
		}

		RefPtrCounter& operator=(RefPtrCounter&& other) noexcept
		{
			if (this == &other)
				return *this;
			m_Count = other.m_Count;
			return *this;
		}

		void AddRef() { ++(*m_Count); }

		std::atomic<int>* GetCount() const { return m_Count; }
		
		bool RemoveRef() { return --(*m_Count) == 0; }

		void DeleteCounter()
		{
			

			if (m_Count && *m_Count)
			{
				delete m_Count;
				m_Count = nullptr;
			}
			
		}



	};

	
}

