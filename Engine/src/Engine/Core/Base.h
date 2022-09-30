﻿#pragma once
#include <forward_list>
#include <memory>

namespace Polyboid
{
    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T>
    using Unique = std::unique_ptr<T>;
    
    
}

