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
    shape::iterator insertion_iterator(shape& shape, cml::vector2f const& point)
    {
        float distance = std::numeric_limits<float>::max();
        shape::iterator iterator;
        auto begin_node = shape.back();
        for (auto i = shape.begin(); i != shape.end(); ++i)
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
    shape::const_iterator wraparound_next(Container const& container, Iterator iterator)
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

shape::iterator insert_point(shape& shape, cml::vector2f const& point)
{
    if (shape.size() < 3)
    {
        shape.push_back({point, point});
        return --shape.end();
    }
    else
    {
        return shape.insert(insertion_iterator(shape, point), {point, point});
    }
}

void erase_points(shape& shape, std::set<size_t> const& indices)
{
    for (auto iter = indices.rbegin(); iter != indices.rend(); ++iter)
    {
        shape.erase(shape.begin() + *iter);
    }
}

point_ref nearest_point(shape& shape, cml::vector2f const& point, float& distance)
{
    point_ref nearest_ref;
    float nearest_distance = std::numeric_limits<float>::max();

    for (auto iter = begin(shape); iter != end(shape); ++iter)
    {
        auto temp = linjal::distance(point, iter->control_point);
        if (temp < nearest_distance)
        {
            nearest_distance = temp;
            nearest_ref.set_control_point(std::distance(begin(shape), iter));
            distance = temp;
        }

        temp = linjal::distance(point, iter->position);
        if (temp < nearest_distance)
        {
            nearest_distance = temp;
            nearest_ref.set_position(std::distance(begin(shape), iter));
            distance = temp;
        }
    }

    return nearest_ref;
}

void shape_curve(shape const& shape, Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera)
{
    if (shape.empty())
    {
        return;
    }

    const float k = 0.551784f;
    cairo_move_to(cairo, camera.to_screen_space(shape.front().position));

    for (shape::const_iterator iter = begin(shape); iter != end(shape); ++iter)
    {
        auto const& node = *iter;
        auto const& next = *wraparound_next(shape, iter);
        auto b = lerp(node.position, node.control_point, k);
        auto c = lerp(next.position, node.control_point, k);
        cairo_curve_to(cairo,
                       camera.to_screen_space(b),
                       camera.to_screen_space(c),
                       camera.to_screen_space(next.position));
    }
}

} // namespace linjal
