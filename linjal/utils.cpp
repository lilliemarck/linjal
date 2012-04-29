#include "utils.hpp"
#include "math.hpp"

namespace linjal {

void cairo_move_to(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& p)
{
    cairo->move_to(p[0], p[1]);
}

void cairo_curve_to(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& p1, cml::vector2f const& p2, cml::vector2f const& p3)
{
    cairo->curve_to(p1[0], p1[1], p2[0], p2[1], p3[0], p3[1]);
}

void cairo_cirlce(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& center, float radius)
{
    cairo->arc(center[0], center[1], radius, 0.0, cml::constantsd::two_pi());
}

} // namespace linjal
