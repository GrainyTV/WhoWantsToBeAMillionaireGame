#pragma once
#include <print>
#include <stacktrace>

#define ASSERT(expression, ...)                                                     \
    {                                                                               \
        if ((expression) == false)                                                  \
        {                                                                           \
            std::println(stderr, "Assertion failed: {}", std::format(__VA_ARGS__)); \
            std::println(stderr, "Called from: {}:{}", __FILE_NAME__, __LINE__);    \
            std::println(stderr, "{}", std::stacktrace::current());                 \
            std::abort();                                                           \
        }                                                                           \
    }

#define LOG(format, ...)                                         \
    {                                                            \
        std::println(stderr, format __VA_OPT__(, ) __VA_ARGS__); \
    }

#ifdef __clang__
#define __TAILREC__ [[clang::musttail]]
#else
#define __TAILREC__
#endif