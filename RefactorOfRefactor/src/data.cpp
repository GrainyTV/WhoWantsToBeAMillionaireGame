#define TOML_IMPLEMENTATION
#include "data.hpp"
#include "debug.hpp"
#include "functionals.hpp"
#include "toml++/toml.hpp"
#include <unordered_map>

namespace Toml
{
    namespace
    {
        Difficulty strToDifficulty(std::string_view text)
        {
            std::unordered_map<std::string_view, Difficulty> conversion =
            {
                { "easy", Difficulty::Easy },
                { "medium", Difficulty::Medium },
                { "hard", Difficulty::Hard },
            };

            ASSERT(conversion.contains(text), "Invalid difficulty encountered ({})", text);
            return conversion[text];
        }
    }

    std::vector<Data> getAllFromFile(std::string_view path)
    {
        const toml::parse_result parse = toml::parse_file(path);

        ASSERT(parse.succeeded(), "Invalid TOML file encountered ({})", path);
        const toml::table& content = parse.table();

        ASSERT(content["entry"].is_array_of_tables(), "File not in 'array of tables' format prefixed with [[entry]] tags ({})", path);
        const toml::array& entries = *content["entry"].as_array();

        return 
            entries
            | Seq::mapi([&](const toml::node& entry, size_t i) -> Toml::Data
                {
                    ASSERT(entry.is_table(), "Entry is not a valid table at index {}", i);
                    const auto properties = *entry.as_table();

                    ASSERT(properties["difficulty"].is_string() &&
                           properties["question"].is_string() &&
                           properties["correctAnswer"].is_string() &&
                           properties["incorrectAnswers"].is_array(), "Entry is misconfigured at index {}", i);

                    const Difficulty difficulty = strToDifficulty(**properties["difficulty"].as_string());
                    const auto question = **properties["question"].as_string();
                    const auto correctAnswer = **properties["correctAnswer"].as_string();
                    const auto incorrectAnswers = *properties["incorrectAnswers"].as_array();

                    ASSERT(incorrectAnswers.size() == 3, "The number of incorrect answers should always be 3, found: {}", incorrectAnswers.size());
                    ASSERT(incorrectAnswers[0].is_string() &&
                           incorrectAnswers[1].is_string() &&
                           incorrectAnswers[2].is_string(), "Incorrect answers should always use string as type<3>, found: ({}, {}, {})",
                                static_cast<uint8_t>(incorrectAnswers[0].type()),
                                static_cast<uint8_t>(incorrectAnswers[1].type()),
                                static_cast<uint8_t>(incorrectAnswers[2].type()));

                    return {
                        .Diff = difficulty,
                        .Question = question,
                        .Answers = { 
                            Answer(correctAnswer, true),
                            Answer(**incorrectAnswers[0].as_string(), false),
                            Answer(**incorrectAnswers[1].as_string(), false),
                            Answer(**incorrectAnswers[2].as_string(), false),
                        }
                    };
                });
    }
}

