#ifndef LINJAL_SHAPE_HPP
#define LINJAL_SHAPE_HPP

#include <cairomm/cairomm.h>
#include <math/vector.hpp>
#include <set>
#include "point_ref.hpp"

namespace linjal {

class camera;

struct node
{
    math::vector2f position;
    math::vector2f control_point;
};

using path = std::vector<node>;

struct shape
{
    size_t color_index;
    linjal::path path;
};

/**
 * Insert the point at the best place in the shape. Return an iterator to
 * the new element.
 */
path::iterator insert_point(path& path, math::vector2f const& point);
void erase_points(path& path, std::set<size_t> const& indices);

point_ref nearest_point(path& path, math::vector2f const& point, float& distance);

void path_curve(path const& path, Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera);

} // namespace linjal

#endif // LINJAL_SHAPE_HPP
