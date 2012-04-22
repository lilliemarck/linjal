#include "math.hpp"

namespace linjal {

float distance(cml::vector2f const& lhs, cml::vector2f const& rhs)
{
    return length(rhs - lhs);
}

} // namespace linjal
