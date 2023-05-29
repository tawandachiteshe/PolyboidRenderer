#pragma once
#include <forward_list>
#include <memory>
#include <utility>

#include "Utils/SmartPtr.h"

namespace Polyboid
{
    template<typename T>
    using Ref = RefPtr<T>;

    class Event;
    
    
    template<typename T>
    using Unique = std::unique_ptr<T>;

	// Credit the cherno Hazel
#define BIND_EVENT(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

    template<typename T, typename O>
    static T CastAs(const O& o)
    {
        T* a = (T*)&o;
        return *a;
    }

    template<typename T>
    static T CastEventAs(const Event& o)
    {
        T* a = (T*)&o;
        return *a;
    }
    
}

