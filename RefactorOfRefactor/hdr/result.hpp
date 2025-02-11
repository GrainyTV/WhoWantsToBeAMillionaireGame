#pragma once
#include "SDL3/SDL.h"
#include "debug.hpp"
#include "option.hpp"
#include <string_view>
#include <type_traits>

template<typename T>
class Result
{
private:
    bool isSuccess;
    Option::Option<T> successValue;
    Option::Option<std::string_view> errorMessage;

    Result(const Result&) = delete;
    Result(Result&&) = delete;

public:
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

    const T& value() const
    {
        ASSERT(isOk(), "Tried to access inner value of result object in error state!");
        return successValue.value();
    }

    const std::string cause() const
    {
        ASSERT(isError(), "Tried to access error message of result object in success state!");
        char buffer[512];
        SDL_snprintf(buffer, sizeof(buffer), "%s (%s)", errorMessage.value().data(), SDL_GetError());
        return std::string(buffer); 
    }
};

template<typename T>
Result(T, std::string) -> Result<T>;

template<typename T>
Result(T, std::string, T) -> Result<T>;

template<typename T>
Result(T, std::string, T, T) -> Result<T>;
