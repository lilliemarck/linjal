#include "camera.hpp"

namespace linjal {

camera::camera() : position_(0.0f, 0.0f), zoom_(1)
{
}

cml::vector2f camera::to_model_space(cml::vector2f const& screen) const
{
    return screen / zoom_ + position_;
}

cml::vector2f camera::to_model_scale(cml::vector2f const& size) const
{
    return size / zoom_;
}

cml::vector2f camera::to_screen_space(cml::vector2f const & model) const
{
    return (model - position_) * zoom_;
}

cml::vector2i camera::get_position()
{
    return position_;
}

void camera::set_position(cml::vector2i const& position)
{
    position_ = position;
}

void camera::set_zoom(int zoom, cml::vector2f const& cursor)
{
    position_ = position_ + cursor / zoom_ - cursor / zoom;
    zoom_ = zoom;
}

int camera::get_zoom() const
{
    return zoom_;
}

} // namespace linjal
