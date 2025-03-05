#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace Toml
{
    namespace _detail
    {
        enum class Difficulty : std::uint8_t
        {
            Easy,
            Medium,
            Hard,
        };

        struct Answer
        {
            std::string Text;
            bool IsCorrect;
        };
    }
    
    using _detail::Difficulty;
    using _detail::Answer;

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
