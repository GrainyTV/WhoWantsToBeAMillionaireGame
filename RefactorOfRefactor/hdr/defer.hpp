#pragma once
#include <functional>

class Defer
{
private:
    std::function<void()> function;

public:    
    template<typename T>
    Defer(void(T::*memberFunction)() const, const T& object)
        : function([&]() { (object.*memberFunction)(); })
    {}

    Defer(void(*staticFunction)())
        : function(staticFunction)
    {}

    ~Defer()
    {
        function();
    }
};