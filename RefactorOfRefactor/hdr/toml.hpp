#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Toml
{
    enum class Difficulty : std::uint8_t
    {
        EASY,
        MEDIUM,
        HARD,
    };

    struct Answer
    {
        std::string Text;
        bool IsCorrect;
    };

    struct Data
    {
        Difficulty Diff;
        std::string Question;
        Answer AnswerA;
        Answer AnswerB;
        Answer AnswerC;
        Answer AnswerD;
    };
    
    std::vector<Data> getAllFromFile(std::string_view);
}
