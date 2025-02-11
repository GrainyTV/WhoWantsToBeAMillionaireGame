#pragma once

#include "debug.hpp"
#include <optional>
#include <string>
#include <typeinfo>

namespace Option
{
    template<typename T>
    class Option
    {
    private:
        std::optional<T> valueOrNone;

    public:
        constexpr Option(const T& value)
            : valueOrNone(value)
        {}

        constexpr Option()
            : valueOrNone(std::nullopt)
        {}
        
        bool isSome() const
        {
            return valueOrNone.has_value();
        }

        bool isNone() const
        {
            return isSome() == false;
        }

        std::string toString() const
        {
            return isSome() ? "Some(" + std::string(typeid(*valueOrNone).name()) + ')' : "None()";
        }

        // T& valueAsReadWrite()
        // {
        //     ASSERT(isSome(), "Tried to access inner value of empty optional!");
        //     return *valueOrNone;
        // }

        const T& value() const
        {
            ASSERT(isSome(), "Tried to access inner value of empty optional!");
            return *valueOrNone;
        }

        // static constexpr Option Some(const T& value)
        // {
        //     return Option(value);
        // }

        // static constexpr Option None()
        // {
        //     return Option();
        // }
    };

    template<typename T>
    constexpr Option<T> Some(const T& value)
    {
        return Option<T>(value);
    }

    template<typename T>
    constexpr Option<T> None()
    {
        return Option<T>();
    }
}

