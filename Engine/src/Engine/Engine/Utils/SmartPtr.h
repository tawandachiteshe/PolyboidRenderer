﻿#pragma once
#include "Allocators.h"
#include "RefCounter.h"


namespace Polyboid
{


	template<typename  T>
	class RefPtr
	{

	private:
		RefPtrCounter m_RefCount;
		T* m_Ptr = nullptr;

	public:

		using ElementType = T;

		constexpr RefPtr() noexcept {}

		constexpr RefPtr(nullptr_t) noexcept {}


		RefPtr(T* ptr, const RefPtrCounter& refCount): m_RefCount(refCount), m_Ptr(ptr)
		{
			
		}

		explicit RefPtr(T* ptr): m_Ptr(ptr)
		{

		}

		RefPtr(const RefPtr<T>& other) : m_RefCount(other.m_RefCount), m_Ptr(other.m_Ptr) {
			m_RefCount.AddRef();
		}
		template<typename V>
		RefPtr(const RefPtr<V>& other, T* ptr) : m_RefCount(other.GetRefCount()), m_Ptr(ptr) {
			m_RefCount.AddRef();
		}


		template<typename U,
			typename std::enable_if<std::is_base_of<T, U>::value && !std::is_same<T, U>::value>::type* = nullptr>
		RefPtr(RefPtr<U>& other) : m_RefCount(other.GetRefCount()), m_Ptr(other.Get()) {
			m_RefCount.AddRef();
		}



		RefPtrCounter GetRefCount() const { return m_RefCount; }

		T* Get() const {
			if (*m_RefCount.GetCount() > 0)
			{

				return m_Ptr;
			}

			__debugbreak();
			return nullptr;
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
				m_Ptr = other.Get();
				m_RefCount = other.GetRefCount();
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



		~RefPtr() {
			if(m_RefCount.RemoveRef())
			{
				if(m_Ptr)
				{
					m_RefCount.DeleteCounter();
				}
			}

		}


	};

	template<typename T, typename... Args>
	RefPtr<T> CreateRef(Args&&... args) {
		return RefPtr<T>(new T(std::forward<Args>(args)...));
	}

	template<typename  T>
	class ScopePtr
	{
	private:
		T* m_Ptr = nullptr;

	public:

		using ElementType = T;

		ScopePtr<T>& operator=(const ScopePtr<T>& other) = delete;
		ScopePtr(const ScopePtr<T>& other) = delete;

		explicit ScopePtr(T* ptr) : m_Ptr(ptr)
		{

		}

		T* Get() const {
			return m_Ptr;
		}

		T* operator->() const {
			return Get();
		}

		T& operator*() const {
			return *Get();
		}


		template<typename U>
		ScopePtr<U> As()
		{
			const auto convertPtr = reinterpret_cast<typename ScopePtr<U>::ElementType*>(m_Ptr);

			ScopePtr<U> other(convertPtr);

			return other;
		}

		~ScopePtr()
		{
			delete m_Ptr;
		}


		template<typename T, typename... Args>
		ScopePtr<T> CreateRef(Args&&... args) {
			return ScopePtr<T>(new T(std::forward<Args>(args)...));
		}

	};


}




