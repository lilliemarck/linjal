#include "view_transform.hpp"

namespace linjal {

view_transform::view_transform() : focus_(0.0f, 0.0f), zoom_(1)
{
}

cml::vector2f view_transform::to_model(cml::vector2f const& screen) const
{
    return screen / zoom_ + focus_;
}

cml::vector2f view_transform::to_model_scale(cml::vector2f const& screen_scale) const
{
    return screen_scale / zoom_;
}

cml::vector2f view_transform::to_screen(cml::vector2f const & model) const
{
    return (model - focus_) * zoom_;
}

cml::vector2f view_transform::get_focus()
{
    return focus_;
}

void view_transform::set_focus(cml::vector2f const& focus)
{
    focus_.set(round(focus[0]), round(focus[1]));
}

void view_transform::set_zoom(int zoom, cml::vector2f const& screen_focus)
{
    focus_ = focus_ + screen_focus / zoom_ - screen_focus / zoom;
    zoom_ = zoom;
}

int view_transform::zoom() const
{
    return zoom_;
}

} // namespace linjal
