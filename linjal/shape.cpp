#include "shape.hpp"
#include <limits>

namespace linjal {

namespace
{
    /**
     * Return the distance between two points.
     */
    float distance(cml::vector2f const& lhs, cml::vector2f const& rhs)
    {
        return length(rhs - lhs);
    }

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
        auto begin_point = shape.back();
        for (auto i = shape.begin(); i != shape.end(); ++i)
        {
            auto const& end_point = *i;
            auto temp = distance_to_line(begin_point, end_point, point);
            if (temp < distance)
            {
                distance = temp;
                iterator = i;
            }
            begin_point = end_point;
        }
        return iterator;
    }
}

void insert_point(shape& shape, cml::vector2f const& point)
{
    if (shape.size() < 3)
    {
        shape.push_back(point);
    }
    else
    {
        shape.insert(insertion_iterator(shape, point), point);
    }
}

} // namespace linjal