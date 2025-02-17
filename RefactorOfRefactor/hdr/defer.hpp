#pragma once
#include "invokable.hpp"

#define CONCAT(a, b) a##b
#define CONCAT2(a, b) CONCAT(a, b)
#define DEFER(function, ...) const Defer CONCAT2(__defer__, __LINE__)(function __VA_OPT__(, ) __VA_ARGS__)

class Defer
{
private:
    Invokable deferredAction;

public:
    template<typename Func, typename... Args>
    explicit Defer(Func&& function, Args&&... arguments)
        : deferredAction(std::forward<Func>(function), std::forward<Args>(arguments)...)
    {}

    ~Defer()
    {
        deferredAction.execute();
    }
};
