#pragma once

#include <math/vector.hpp>
#include <set>
#include <vector>
#include "point_ref.hpp"

namespace linjal {

class camera;
class drawing_context;

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

void path_curve(path const& path, drawing_context& context, camera const& camera);

} // namespace linjal
