#pragma once
#include <cassert>
#include "Allocators.h"

namespace Polyboid
{


	template<typename T>
	class RefPtrCounter
	{

	private:
		std::atomic_int32_t* m_Count;
		T* m_Ptr;

	public:

		RefPtrCounter(): m_Count(new std::atomic_int32_t(1)), m_Ptr(nullptr) {  }

		RefPtrCounter(T* ptr): m_Count(new std::atomic_int32_t(1)), m_Ptr(ptr) {   }

		template<typename V>
		RefPtrCounter(const RefPtrCounter<V>& other, T* ptr) : m_Count(other.GetCount()), m_Ptr(ptr) {   }


		template<typename U,
			typename std::enable_if<std::is_base_of<T, U>::value && !std::is_same<T, U>::value>::type* = nullptr>
		RefPtrCounter(const RefPtrCounter<U>& other) : m_Count(other.GetCount()), m_Ptr(other.Get())
		{
			
		}

		void AddRef() { ++(*m_Count); }

		std::atomic<int>* GetCount() const { return m_Count; }
		
		bool RemoveRef() { return --(*m_Count) == 0; }

		void DeleteCounter()
		{
			
			
			if (m_Ptr)
			{
				delete m_Ptr;
				m_Ptr = nullptr;
			}

			if (m_Count && *m_Count)
			{
				delete m_Count;
				m_Count = nullptr;
			}
			
		}

		T* Get() const {
			assert(*m_Count > 0);
			return m_Ptr;
		}




	};

	
}

