#pragma once
#include <functional>

class Invokable
{
private:
    std::function<void()> action;

public:
    template<typename Func, typename... Args>
    requires std::invocable<Func, Args...> && std::is_void_v<std::invoke_result_t<Func, Args...>>
    Invokable(Func&& function, Args&&... arguments)
        : action([&]() { std::invoke(std::forward<Func>(function), std::forward<Args>(arguments)...); })
    {}

    void execute() const
    {
        action();
    }
};