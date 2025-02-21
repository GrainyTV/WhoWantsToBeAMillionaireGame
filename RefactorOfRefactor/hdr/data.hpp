#pragma once
#include "functionals.hpp"
#include "result.hpp"
#include "toml++/toml.hpp"
#include <array>
#include <cstdint>
#include <string_view>

enum class Difficulty : std::uint8_t
{
    Easy,
    Medium,
    Hard,
};

struct Answer
{
    std::string_view Text;
    bool IsCorrect;
};

struct Data
{
    Difficulty diff;
    std::string_view question;
    std::array<Answer, 4> answers;

    static Data convertFromToml(const toml::node&, size_t);
};
