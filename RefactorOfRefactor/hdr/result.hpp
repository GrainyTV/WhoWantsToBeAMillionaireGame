#pragma once
#include "SDL3/SDL.h"
#include "debug.hpp"
#include "optionwrap.hpp"
#include <string_view>
#include <type_traits>

template<typename T>
class Result
{
private:
    bool isSuccess;
    Option::Inst<T> successValue;
    Option::Inst<std::string_view> errorMessage;

public:
    Result(const Result&) = delete;
    Result(Result&&) = delete;

    template<typename U = T>
    requires std::is_same_v<T, bool>
    explicit Result(const U booleanExpression, std::string_view errorMessage)
        : isSuccess(booleanExpression)
        , successValue(isSuccess ? Option::Some(true) : Option::None<T>())
        , errorMessage(isSuccess ? Option::None<std::string_view>() : Option::Some(errorMessage))
    {}
    
    template<typename U = T>
    requires std::is_same_v<T, uint32_t>
    explicit Result(const U integerExpression, std::string_view errorMessage, const U shouldBe = 0)
        : isSuccess(integerExpression == shouldBe)
        , successValue(isSuccess ? Option::Some(integerExpression) : Option::None<T>())
        , errorMessage(isSuccess ? Option::None<std::string_view>() : Option::Some(errorMessage))
    {}

    template<typename U = T>
    requires std::is_same_v<T, int64_t>
    explicit Result(const U integerExpression, std::string_view errorMessage, const U min, const U max)
        : isSuccess(integerExpression >= min && integerExpression <= max) 
        , successValue(isSuccess ? Option::Some(integerExpression) : Option::None<T>())
        , errorMessage(isSuccess ? Option::None<std::string_view>() : Option::Some(errorMessage))
    {}

    template<typename U = T>
    requires std::is_pointer_v<T>
    explicit Result(const U pointerExpression, std::string_view errorMessage)
        : isSuccess(pointerExpression != nullptr)
        , successValue(isSuccess ? Option::Some(pointerExpression) : Option::None<T>())
        , errorMessage(isSuccess ? Option::None<std::string_view>() : Option::Some(errorMessage))
    {}

    bool isOk() const
    {
        return isSuccess;
    }

    bool isError() const
    {
        return isOk() == false;
    }

    const T& unwrap() const
    {
        Debug::assert(isOk(), "Tried to access inner value of result object in error state!");
        return successValue.value();
    }

    std::string toString() const
    {
        return isOk() ? "Ok()" : "Error(" + std::string(errorMessage.value()) + ")";
    }
};

template<typename T>
Result(T, std::string) -> Result<T>;

template<typename T>
Result(T, std::string, T) -> Result<T>;

template<typename T>
Result(T, std::string, T, T) -> Result<T>;
