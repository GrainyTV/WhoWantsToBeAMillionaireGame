#pragma once
#include <format>
#include <print>

#define ASSERT(expression, ...) { \
    if (expression == false) { \
        std::println("Assertion failed: {}", std::format(__VA_ARGS__)); \
        std::abort(); \
    } \
}