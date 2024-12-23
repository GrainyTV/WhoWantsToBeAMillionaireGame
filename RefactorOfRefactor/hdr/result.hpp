#pragma once
#include "SDL3/SDL.h"
#include "debug.hpp"
#include <string>
#include <type_traits>
#include <variant>

template<typename T>
class Result
{
private:
    std::variant<T, std::string> valueOrError;

    Result(const Result&) = delete;

    Result(Result&&) = delete;

public:
    Result(const int32_t result, const std::string& failMessage, const int32_t shouldBe = 0)
        : valueOrError(result == shouldBe
                           ? std::variant<int32_t, std::string>(std::in_place_type<int32_t>, result)
                           : std::variant<int32_t, std::string>(std::in_place_type<std::string>, failMessage))
    {}

    Result(const int32_t result, const std::string& failMessage, const int32_t min, const int32_t max)
        : valueOrError((result >= min && result <= max) 
                           ? std::variant<int32_t, std::string>(std::in_place_type<int32_t>, result)
                           : std::variant<int32_t, std::string>(std::in_place_type<std::string>, failMessage))
    {}

    template<typename U>
    requires std::is_pointer_v<T> && std::is_same_v<T, U>
    Result(const U& result, const std::string& failMessage)
        : valueOrError(result != nullptr
                           ? std::variant<U, std::string>(std::in_place_type<U>, result)
                           : std::variant<U, std::string>(std::in_place_type<std::string>, failMessage))
    {}

    bool isOk() const
    {
        return std::holds_alternative<T>(valueOrError);
    }

    bool isError() const
    {
        return isOk() == false;
    }

    void assertOk() const
    {
        ASSERT(isOk(), "{} ({})", std::get<std::string>(valueOrError), SDL_GetError());
    }

    const T& value() const
    {
        ASSERT(isOk(), "Tried to access inner value of result object in error state!");
        return std::get<T>(valueOrError);
    }
};
