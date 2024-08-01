#pragma once
#include <print>

#define ASSERT(expression, ...)                                                     \
    {                                                                               \
        if ((expression) == false)                                                  \
        {                                                                           \
            std::println(stderr, "Assertion failed: {}", std::format(__VA_ARGS__)); \
            std::println(stderr, "{}:{}", __FILE__, __LINE__);                      \
            std::abort();                                                           \
        }                                                                           \
    }

#define LOG(format, ...)                                         \
    {                                                            \
        std::println(stderr, format __VA_OPT__(, ) __VA_ARGS__); \
    }
