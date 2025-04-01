#pragma once
#include "cpptrace/cpptrace.hpp"
#include "fmt/base.h"
#include "fmt/format.h"

namespace Debug
{
    consteval bool isDebugMode()
    {
        #ifndef NDEBUG
            return true;
        #else
            return false;
        #endif
    }

    template<bool Enabled = isDebugMode(), typename... Args>
    void assert(bool expr, fmt::format_string<Args...> message, Args&&... args)
    {
        if constexpr (Enabled)
        {
            if (expr == false)
            {
                fmt::println(stderr, "Assertion failed: {}", fmt::format(message, std::forward<Args>(args)...));
                cpptrace::generate_trace(1).print_with_snippets();
                std::abort();
            }
        }
    }

    template<bool Enabled = isDebugMode(), typename... Args>
    void log(fmt::format_string<Args...> message, Args&&... args)
    {
        if constexpr (Enabled)
        {
            fmt::println(stderr, message, std::forward<Args>(args)...);
        }
    }
}
