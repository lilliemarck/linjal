#ifndef LINJAL_SHAPE_HPP
#define LINJAL_SHAPE_HPP

#include <cml/cml.h>

namespace linjal {

struct node
{
    cml::vector2f position;
    cml::vector2f control_point;
};

typedef std::vector<node> shape;

/**
 * Insert the point at the best place in the shape. Return an iterator to
 * the new element.
 */
shape::iterator insert_point(shape& shape, cml::vector2f const& point);

shape::iterator nearest_point(shape& shape, cml::vector2f const& point);

} // namespace linjal

#endif // LINJAL_SHAPE_HPP
