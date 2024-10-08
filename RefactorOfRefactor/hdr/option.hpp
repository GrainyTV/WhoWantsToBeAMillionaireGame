#pragma once

#include "debug.hpp"
#include <optional>
#include <string>
#include <typeinfo>

template<typename T>
class Option
{
private:
    std::optional<T> valueHolder;

    constexpr Option(const T& valueToHold)
        : valueHolder(valueToHold)
    {}

    constexpr Option()
        : valueHolder(std::nullopt)
    {}

    //Option(const Option&) = delete;

public:

    //Option<T>& operator=(const Option<T>&) = default;

    bool isSome() const
    {
        return valueHolder.has_value();
    }

    bool isNone() const
    {
        return isSome() == false;
    }

    std::string toString() const
    {
        return isSome() ? "Some(" + std::string(typeid(*valueHolder).name()) + ')' : "None()";
    }

    T& valueAsReadWrite()
    {
        ASSERT(isSome(), "Tried to access inner value of empty optional!");
        return *valueHolder;
    }

    const T& value() const
    {
        ASSERT(isSome(), "Tried to access inner value of empty optional!");
        return *valueHolder;
    }

    static constexpr Option Some(const T& valueToHold)
    {
        return Option(valueToHold);
    }

    static constexpr Option None()
    {
        return Option();
    }
};
