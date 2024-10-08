#pragma once

// #include "SDL3/SDL_error.h"
// #include "debug.hpp"
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

    template<typename U>
    requires std::is_pointer_v<T> && std::is_same_v<T, U>
    Result(const U& result, const std::string& failMessage)
        : valueOrError(result != nullptr
                           ? std::variant<U, std::string>(std::in_place_type<U>, result)
                           : std::variant<U, std::string>(std::in_place_type<std::string>, failMessage))
    {}

    bool isOk() const
    {
        return valueOrError.has_value();
    }

    bool isError() const
    {
        return isOk() == false;
    }

    void printError() const
    {
        if (isError())
        {
            // LOG("Error({})", valueOrError.error());
        }
    }

    void assertOk() const
    {
        // ASSERT(isOk(), "{} ({})", valueOrError.error(), SDL_GetError());
    }

    const T& value() const
    {
        // ASSERT(isOk(), "Tried to access inner value of result object in error state!");
        return std::get<0>(valueOrError);
    }
};
