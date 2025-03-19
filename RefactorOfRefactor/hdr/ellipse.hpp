#pragma once
#include "glm/glm.hpp"
#include <vector>

namespace Ellipse
{
    namespace _impl_details
    {
        struct EllipseDescriptor
        {
            glm::vec2 Center;
            float RadiusHorizontal;
            float RadiusVertical;
        };
    }

    std::vector<glm::vec2> generate(const _impl_details::EllipseDescriptor&);
}
