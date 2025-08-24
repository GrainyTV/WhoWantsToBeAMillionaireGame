#include "border.hpp"
#include "seq/seq.hpp"

namespace Border
{
	constexpr auto generateFrom(float thickness, std::span<const glm::vec2> points)
	{
        std::array<vec2, outputvertexcount> result;

		auto createquadfromendpoints = [&](const std::pair<vec2, vec2>& pair, const size_t i)
		{
		    const vec2& startvertex = pair.first;
		    const vec2& endvertex = pair.second;

		    const vec2 direction = endvertex - startvertex;
		    vec2 perpendicular(-direction.y, direction.x);
		    perpendicular = glm::normalize(perpendicular);
		    perpendicular *= border_thickness / 2.0f;

		    const size_t pairstride = i * 4;

		    result[pairstride + 0] = startvertex + perpendicular;
		    result[pairstride + 1] = startvertex - perpendicular;
		    result[pairstride + 2] = endvertex + perpendicular;
		    result[pairstride + 3] = endvertex - perpendicular;
		};

		const auto neighbors = positions | Seq::pairwiseWrap();
		| Seq::iteri(createquadfromendpoints);

		return result;
	}
}
