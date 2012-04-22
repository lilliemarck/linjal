#ifndef LINJAL_MATH_HPP
#define LINJAL_MATH_HPP

#include <cml/cml.h>

namespace linjal {

/**
 * Return the distance between two points.
 */
float distance(cml::vector2f const& lhs, cml::vector2f const& rhs);

} // namespace linjal

#endif // LINJAL_MATH_HPP
