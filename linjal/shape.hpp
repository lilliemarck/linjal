#ifndef LINJAL_SHAPE_HPP
#define LINJAL_SHAPE_HPP

#include <cairomm/cairomm.h>
#include <cml/cml.h>
#include <set>
#include "point_ref.hpp"

namespace linjal {

class camera;

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
void erase_points(shape& shape, std::set<size_t> const& indices);

point_ref nearest_point(shape& shape, cml::vector2f const& point, float& distance);

void shape_curve(shape const& shape, Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera);

} // namespace linjal

#endif // LINJAL_SHAPE_HPP
