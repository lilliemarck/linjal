#include "shape.hpp"
#include <limits>
#include "camera.hpp"
#include "math.hpp"
#include "utils.hpp"

namespace linjal {

namespace
{
    /**
     * Return the nearest point on the line segment.
     */
    cml::vector2f nearest_point_on_line(cml::vector2f const& begin,
                                        cml::vector2f const& end,
                                        cml::vector2f const& point)
    {
        auto b2e = end - begin;
        auto b2p = point -  begin;
        auto nom = dot(b2p, b2e);
        auto denom = dot(b2e, b2e);

        if (denom == 0.0)
        {
            return begin;
        }

        auto u = nom / denom;

        if (u <= 0.0)
        {
            return begin;
        }
        else if (u >= 1.0)
        {
            return end;
        }
        else
        {
            return begin + u * b2e;
        }
    }

    /**
     * Return the distance to a line segment.
     */
    float distance_to_line(cml::vector2f const& begin,
                           cml::vector2f const& end,
                           cml::vector2f const& point)
    {
        return distance(point, nearest_point_on_line(begin, end, point));
    }

    /**
     * Return an iterator to the best place to insert point.
     */
    path::iterator insertion_iterator(path& path, cml::vector2f const& point)
    {
        float distance = std::numeric_limits<float>::max();
        path::iterator iterator;
        auto begin_node = path.back();
        for (auto i = begin(path); i != end(path); ++i)
        {
            auto const& end_node = *i;
            auto temp = distance_to_line(begin_node.position, end_node.position, point);
            if (temp < distance)
            {
                distance = temp;
                iterator = i;
            }
            begin_node = end_node;
        }
        return iterator;
    }

    template <typename Container, typename Iterator>
    path::const_iterator wraparound_next(Container const& container, Iterator iterator)
    {
        if (++iterator != end(container))
        {
            return iterator;
        }
        else
        {
            return begin(container);
        }
    }
}

path::iterator insert_point(path& path, cml::vector2f const& point)
{
    if (path.size() < 3)
    {
        path.push_back({point, point});
        return --path.end();
    }
    else
    {
        return path.insert(insertion_iterator(path, point), {point, point});
    }
}

void erase_points(path& path, std::set<size_t> const& indices)
{
    for (auto iter = indices.rbegin(); iter != indices.rend(); ++iter)
    {
        path.erase(path.begin() + *iter);
    }
}

point_ref nearest_point(path& path, cml::vector2f const& point, float& distance)
{
    point_ref nearest_ref;
    float nearest_distance = std::numeric_limits<float>::max();

    for (auto iter = begin(path); iter != end(path); ++iter)
    {
        auto temp = linjal::distance(point, iter->control_point);
        if (temp < nearest_distance)
        {
            nearest_distance = temp;
            nearest_ref.set_control_point(std::distance(begin(path), iter));
            distance = temp;
        }

        temp = linjal::distance(point, iter->position);
        if (temp < nearest_distance)
        {
            nearest_distance = temp;
            nearest_ref.set_position(std::distance(begin(path), iter));
            distance = temp;
        }
    }

    return nearest_ref;
}

void path_curve(path const& path, Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera)
{
    if (path.empty())
    {
        return;
    }

    const float k = 0.551784f;
    cairo_move_to(cairo, camera.to_screen_space(path.front().position));

    for (path::const_iterator iter = begin(path); iter != end(path); ++iter)
    {
        auto const& node = *iter;
        auto const& next = *wraparound_next(path, iter);
        auto b = lerp(node.position, node.control_point, k);
        auto c = lerp(next.position, node.control_point, k);
        cairo_curve_to(cairo,
                       camera.to_screen_space(b),
                       camera.to_screen_space(c),
                       camera.to_screen_space(next.position));
    }
}

} // namespace linjal
