#include "camera.hpp"

namespace linjal {

camera::camera() : position_(0, 0), zoom_(1)
{
}

math::vector2f camera::to_model_space(math::vector2f const& screen) const
{
    return screen / float(zoom_) + math::vector2f(position_);
}

math::vector2f camera::to_model_scale(math::vector2f const& size) const
{
    return size / float(zoom_);
}

math::vector2f camera::to_screen_space(math::vector2f const & model) const
{
    return (model - math::vector2f(position_)) * float(zoom_);
}

math::vector2f camera::to_screen_scale(math::vector2f const& size) const
{
    return size * float(zoom_);
}

math::vector2i camera::get_position() const
{
    return position_;
}

void camera::set_position(math::vector2i const& position)
{
    position_ = position;
}

void camera::set_zoom(int zoom, math::vector2f const& cursor)
{
    position_ = position_ + math::vector2i(cursor / float(zoom_) - cursor / float(zoom));
    zoom_ = zoom;
}

int camera::get_zoom() const
{
    return zoom_;
}

} // namespace linjal
