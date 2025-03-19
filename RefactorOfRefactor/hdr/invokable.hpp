#pragma once
#include <functional>

template<typename Func, typename... Args>
concept Action = 
    std::is_invocable_v<Func, Args...> &&
    std::is_void_v<std::invoke_result_t<Func, Args...>>;

class Invokable
{
private:
    std::function<void()> action;

public:
    Invokable() = default;
    
    template<typename Func, typename... Args>
    requires Action<Func, Args...>
    explicit Invokable(Func&& function, Args&&... arguments)
        : action([func = std::forward<Func>(function), ...args = std::forward<Args>(arguments)]() mutable
            {
                std::invoke(func, std::forward<Args>(args)...);
            })
    {}

    void execute() const
    {
        action();
    }
};
