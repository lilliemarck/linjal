#pragma once

#include <math/vector.hpp>

namespace linjal {

/**
 * Represents a reference to either a node's position or its control point.
 */
class point_ref
{
public:
    enum ref_type
    {
        null,
        position,
        control_point
    };

    point_ref();
    size_t operator*() const;

    void set_position(size_t index);
    void set_control_point(size_t index);
    void clear();
    ref_type type() const;

private:
    ref_type type_;
    size_t index_;
};

} // namespace linjal
