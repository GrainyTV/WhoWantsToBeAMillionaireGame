#pragma once
#include "debug.hpp"
#include <optional>
//#include <utility>

//#include <string>
//#include <typeinfo>
//#include <utility>
//namespace Option
//{
//    template<typename T>
//    class Inst;
//    
//    template<typename T>
//    Inst<T> Some(T value)
//    {
//        return Inst<T>(value);
//    }
//
//    template<typename T>
//    Inst<T> None()
//    {
//        return Inst<T>();
//    }
//
//    template<typename T>
//    class Inst
//    {
//    private:
//        std::optional<T> valueOrNone;
//
//        explicit Inst(T value)
//            : valueOrNone(std::move(value))
//        {}
//
//        explicit Inst()
//            : valueOrNone(std::nullopt)
//        {}
//
//    public:
//        bool isSome() const
//        {
//            return valueOrNone.has_value();
//        }
//
//        bool isNone() const
//        {
//            return isSome() == false;
//        }
//
//        std::string toString() const
//        {
//            return isSome() ? "Some(" + std::string(typeid(*valueOrNone).name()) + ')' : "None()";
//        }
//
//        const T& value() const
//        {
//            Debug::assert(isSome(), "Tried to access inner value of empty optional!");
//            return *valueOrNone;
//        }
//
//        friend Inst Some<T>(T);
//
//        friend Inst None<T>();
//    };
//}


// forward declaration of Option
// to make it work with factory functions

template<typename T>
class Option;

namespace OptionExtensions
{
    template<typename T>
    auto None() -> Option<T>
    {
        return Option<T>();
    }

    template<typename T>
    auto Some(const T& value) -> Option<T>
    {
        return Option<T>(value);
    }
}

template<typename T>
class Option
{
private:
    std::optional<T> memberOrNone;

    Option()
        : memberOrNone(std::nullopt)
    {}

    Option(const T& value)
        : memberOrNone(value)
    {}

public:
    auto isSome() const -> bool
    {
        return memberOrNone.has_value();
    }

    auto isNone() const -> bool
    {
        return !isSome();
    }

    auto value() const -> const T&
    {
        Debug::assert(isSome(), "Tried to access inner value of empty optional!");
        return *memberOrNone;
    }

    template<typename U>
    friend auto OptionExtensions::None() -> Option<U>;

    template<typename U>
    friend auto OptionExtensions::Some(const U& value) -> Option<U>;
};
