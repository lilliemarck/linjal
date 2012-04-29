#ifndef LINJAL_UTILS_HPP
#define LINJAL_UTILS_HPP

#include <cairomm/context.h>
#include <cml/cml.h>

namespace linjal {

void cairo_move_to(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& p);
void cairo_curve_to(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& p1, cml::vector2f const& p2, cml::vector2f const& p3);
void cairo_cirlce(Cairo::RefPtr<Cairo::Context> const& cairo, cml::vector2f const& center, float radius);

} // namespace linjal

#endif // LINJAL_UTILS_HPP
