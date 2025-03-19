#pragma once
#include "debug.hpp"
#include <optional>
#include <string>
#include <typeinfo>
#include <utility>

namespace Option
{
    template<typename T>
    class Inst;
    
    template<typename T>
    Inst<T> Some(T value)
    {
        return Inst<T>(value);
    }

    template<typename T>
    Inst<T> None()
    {
        return Inst<T>();
    }

    template<typename T>
    class Inst
    {
    private:
        std::optional<T> valueOrNone;

        explicit Inst(T value)
            : valueOrNone(std::move(value))
        {}

        explicit Inst()
            : valueOrNone(std::nullopt)
        {}

    public:
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

        const T& value() const
        {
            Debug::assert(isSome(), "Tried to access inner value of empty optional!");
            return *valueOrNone;
        }

        friend Inst Some<T>(T);

        friend Inst None<T>();
    };
}


