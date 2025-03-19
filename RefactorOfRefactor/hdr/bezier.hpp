#pragma once
#include "glm/glm.hpp"
#include <span>
#include <vector>

namespace Bezier
{
    std::vector<glm::vec2> generateFromControls(std::span<const glm::vec2>);
}
