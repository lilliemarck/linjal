#include "utils.hpp"

namespace linjal {

void cairo_cirlce(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& center, float radius)
{
    cairo->arc(center[0], center[1], radius, 0.0, cml::constantsd::two_pi());
}

} // namespace linjal
