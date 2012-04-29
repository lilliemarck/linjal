#include "point_ref.hpp"

namespace linjal {

point_ref::point_ref() : type_(null)
{
}

size_t point_ref::operator*() const
{
    return index_;
}

void point_ref::set_position(size_t index)
{
    type_ = position;
    index_ = index;
}

void point_ref::set_control_point(size_t index)
{
    type_ = control_point;
    index_ = index;
}

void point_ref::clear()
{
    type_ = null;
}

point_ref::ref_type point_ref::type() const
{
    return type_;
}

} // namespace linjal
