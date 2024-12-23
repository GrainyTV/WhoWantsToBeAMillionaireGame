#pragma once
#include <functional>

class Invokable
{
private:
    std::function<void()> action;

public:
    Invokable()
    {}

    template<typename Func, typename... Args>
    requires std::invocable<Func, Args...> && std::is_void_v<std::invoke_result_t<Func, Args...>>
    Invokable(Func&& function, Args&&... arguments)
        : action(std::bind(std::forward<Func>(function), std::forward<Args>(arguments)...))
    {}

    void execute() const
    {
        action();
    }

    // Invokable(const Invokable& other)
    // {
    //     (*this).action = other.action;
    // }

    // Invokable& operator=(const Invokable& other) noexcept
    // {
    //     if (this != &other)
    //     {
    //         (*this).action = other.action;
    //     }
        
    //     return *this;
    // }
};