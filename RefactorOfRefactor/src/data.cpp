#define TOML_IMPLEMENTATION
#include "data.hpp"

Data Data::convertFromToml(const toml::node& entry, const size_t index)
{
    ASSERT(entry.is_table(), "Entry is not a valid table at index {}", index);
    const auto properties = *entry.as_table();

    ASSERT(properties["difficulty"].is_string() &&
           properties["question"].is_string() &&
           properties["correctAnswer"].is_string() &&
           properties["incorrectAnswers"].is_array(), "Entry is misconfigured at index {}", index);

    //const auto difficulty = properties["difficulty"].value<std::string_view>();
    const auto question = properties["question"].value<std::string_view>();
    const auto correctAnswer = properties["correctAnswer"].value<std::string_view>();
    const auto incorrectAnswers = *properties["incorrectAnswers"].as_array();

    ASSERT(incorrectAnswers.size() == 3 &&
           incorrectAnswers[0].is_string() &&
           incorrectAnswers[1].is_string() &&
           incorrectAnswers[2].is_string(), "The number of incorrect answers should always be 3 and of type string, found: {}", incorrectAnswers.size());

    return {
        .diff = Difficulty::Easy,
        .question = question.value(),
        .answers = { 
            Answer(correctAnswer.value(), true),
            Answer(incorrectAnswers[0].value<std::string_view>().value(), false),
            Answer(incorrectAnswers[1].value<std::string_view>().value(), false),
            Answer(incorrectAnswers[2].value<std::string_view>().value(), false),
        }
    };
}
