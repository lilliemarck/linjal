#include "shape.hpp"
#include <limits>
#include "math.hpp"

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

shape::iterator nearest_point(shape& shape, cml::vector2f const& point)
{
    shape::iterator neaest_iterator = shape.end();
    float nearest_distance = std::numeric_limits<float>::max();

    for (auto iter = begin(shape); iter != end(shape); ++iter)
    {
        auto temp = distance(point, iter->position);
        if (temp < nearest_distance)
        {
            nearest_distance = temp;
            neaest_iterator = iter;
        }
    }

    return neaest_iterator;
}

} // namespace linjal
