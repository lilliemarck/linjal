#ifndef LINJAL_SHAPE_HPP
#define LINJAL_SHAPE_HPP

#include <cml/cml.h>

namespace linjal {

typedef std::vector<cml::vector2f> shape;

/**
 * Insert the point at the best place in the shape.
 */
void insert_point(shape& shape, cml::vector2f const& point);

} // namespace linjal

#endif // LINJAL_SHAPE_HPP
