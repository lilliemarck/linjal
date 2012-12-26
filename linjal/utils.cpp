#include "utils.hpp"
#include <math/math.hpp>

namespace linjal {

void cairo_move_to(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& p)
{
    cairo->move_to(p.x(), p.y());
}

void cairo_curve_to(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& p1, math::vector2f const& p2, math::vector2f const& p3)
{
    cairo->curve_to(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
}

void cairo_cirlce(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& center, float radius)
{
    cairo->arc(center.x(), center.y(), radius, 0.0, math::tau);
}

} // namespace linjal
