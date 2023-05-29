#pragma once
#include "RefCounter.h"


namespace Polyboid
{


	template<typename  T>
	class RefPtr
	{

	private:
		RefPtrCounter<T> m_RefCount;
		T* m_Ptr = nullptr;

	public:

		using ElementType = T;

		constexpr RefPtr() noexcept = default;

		constexpr RefPtr(nullptr_t) noexcept {}


		RefPtr(T* ptr, const RefPtrCounter<T>& refCount): m_RefCount(refCount), m_Ptr(ptr)
		{
			
		}

		explicit RefPtr(T* ptr) : m_RefCount(ptr), m_Ptr(ptr)
		{

		}

		RefPtr(const RefPtr<T>& other) : m_RefCount(other.m_RefCount), m_Ptr(other.m_Ptr) {
			m_RefCount.AddRef();
		}
		template<typename V>
		RefPtr(const RefPtr<V>& other, T* ptr) : m_RefCount(other.GetRefCount(), ptr), m_Ptr(ptr) {
			m_RefCount.AddRef();
		}


		template<typename U,
			typename std::enable_if<std::is_base_of<T, U>::value && !std::is_same<T, U>::value>::type* = nullptr>
		RefPtr(RefPtr<U>& other) : m_RefCount(other.GetRefCount()), m_Ptr(other.Get()) {
			m_RefCount.AddRef();
		}



		RefPtrCounter<T> GetRefCount() const { return m_RefCount; }

		T* Get() const {
			return m_RefCount.Get();
		}

		T* operator->() const {
			return Get();
		}

		T& operator*() const {
			return *Get();
		}



		template<typename U>
		RefPtr<U> As()
		{
			const auto convertPtr = reinterpret_cast<typename RefPtr<U>::ElementType*>(m_Ptr);

			RefPtr<U> other(*this, convertPtr);
			auto otherPtr = other.Get();
			
			//std::swap(otherPtr, m_Ptr);
			

			return other;
		}

		template<typename U>
		RefPtr<U> As() const
		{
			const auto convertPtr = reinterpret_cast<typename RefPtr<U>::ElementType*>(m_Ptr);

			RefPtr<U> other(*this, convertPtr);
			auto otherPtr = other.Get();


			return other;
		}


		RefPtr& operator=(const RefPtr& other) {

			if (this != &other) {
				m_RefCount.RemoveRef();
				m_Ptr = other.m_Ptr;
				m_RefCount = other.m_RefCount;
				m_RefCount.AddRef();
			}
			return *this;
		}


		template<typename U,
			typename std::enable_if<std::is_base_of<T, U>::value && !std::is_same<T, U>::value>::type* = nullptr>
		RefPtr<T>& operator=(const RefPtr<U>& other) {

			if (this != &other) {
				m_RefCount.RemoveRef();
				m_Ptr = other.Get();
				m_RefCount = other.GetRefCount();
				m_RefCount.AddRef();
			}
			return *this;
		}

		template<typename U>
		RefPtr& operator=(RefPtr<U>&& other) {

			if (this != &other) {
				m_RefCount.RemoveRef();
				m_Ptr = other.m_Ptr;
				m_RefCount = other.m_RefCount;
				m_RefCount.AddRef();
			}
			return *this;
		}





		~RefPtr() {
			if(m_RefCount.RemoveRef())
			{
				delete m_Ptr;
				m_RefCount.DeleteCounter();
			}
		}


	};

	template<typename T, typename... Args>
	RefPtr<T> CreateRef(Args&&... args) {
		return RefPtr<T>(new T(std::forward<Args>(args)...));
	}


	class ScopePtr
	{
	public:

	};


}




