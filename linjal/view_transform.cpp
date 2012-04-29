#include "view_transform.hpp"

namespace linjal {

view_transform::view_transform() : zoom_(1)
{
}

cml::vector2f view_transform::to_model(const cml::vector2f& screen) const
{
    return screen / zoom_;
}

cml::vector2f view_transform::to_screen(const cml::vector2f& model) const
{
    return model * zoom_;
}

void view_transform::zoom_in()
{
    ++zoom_;
}

void view_transform::zoom_out()
{
    if (zoom_ > 1)
    {
        --zoom_;
    }
}

int view_transform::zoom() const
{
    return zoom_;
}

} // namespace linjal
