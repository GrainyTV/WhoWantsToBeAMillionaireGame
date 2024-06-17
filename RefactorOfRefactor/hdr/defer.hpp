#pragma once
#include <functional>
#include <utility>

#define CONCAT(a, b) a##b
#define CONCAT2(a, b) CONCAT(a, b)
#define DEFER(function, ...) const Defer CONCAT2(__defer__, __LINE__)(function __VA_OPT__(, ) __VA_ARGS__)

class Defer
{
private:
    std::function<void()> deferredAction;

public:
    /*template<typename Func, typename... Args>
    Defer(Func function, Args&&... arguments)
        : deferredAction([&, function]() { std::invoke(function, std::forward<Args>(arguments)...); })
    {}

    template<typename Func, typename Obj, typename... Args>
    Defer(Func function, Obj& object, Args&&... arguments)
        : deferredAction([&, function]() { std::invoke(function, object, std::forward<Args>(arguments)...); })
    {}*/

    template<typename Func, typename... Args>
    Defer(Func&& function, Args&&... arguments)
        : deferredAction([&]() { std::invoke(std::forward<Func>(function), std::forward<Args>(arguments)...); })
    {}

    ~Defer()
    {
        deferredAction();
    }
};