#pragma once

#ifndef NDEBUG
    #include "cpptrace/cpptrace.hpp"
    #include "fmt/base.h"
    #include "fmt/format.h"

    #define ASSERT(expression, ...)                                                 \
    {                                                                               \
        if ((expression) == false)                                                  \
        {                                                                           \
            fmt::println(stderr, "Assertion failed: {}", fmt::format(__VA_ARGS__)); \
            cpptrace::generate_trace().print_with_snippets();                       \
            std::abort();                                                           \
        }                                                                           \
    }

    #define LOG(format, ...)                                     \
    {                                                            \
        fmt::println(stderr, format __VA_OPT__(, ) __VA_ARGS__); \
    }

#else
    #define ASSERT(expression, ...) ((void)(expression))
    #define LOG(format, ...)

#endif

#ifdef __clang__
    #define __TAILREC__ [[clang::musttail]]

#else
    #define __TAILREC__

#endif
