#ifndef LINJAL_UTILS_HPP
#define LINJAL_UTILS_HPP

#include <cairomm/context.h>
#include <math/vector.hpp>

namespace linjal {

void cairo_move_to(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& p);
void cairo_curve_to(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& p1, math::vector2f const& p2, math::vector2f const& p3);
void cairo_cirlce(Cairo::RefPtr<Cairo::Context> const& cairo, math::vector2f const& center, float radius);

} // namespace linjal

#endif // LINJAL_UTILS_HPP
