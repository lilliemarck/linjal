#ifndef LINJAL_VIEW_TRANSFORM_HPP
#define LINJAL_VIEW_TRANSFORM_HPP

#include <cml/cml.h>

namespace linjal {

class view_transform
{
public:
    view_transform();
    cml::vector2f to_model(cml::vector2f const& screen) const;
    cml::vector2f to_model_scale(cml::vector2f const& screen_scale) const;
    cml::vector2f to_screen(cml::vector2f const& model) const;

    cml::vector2f get_focus();
    void set_focus(cml::vector2f const& focus);

    void set_zoom(int zoom, cml::vector2f const& screen_focus);
    int zoom() const;

private:
    cml::vector2f focus_;
    int zoom_;
};

} // namespace linjal

#endif // LINJAL_VIEW_TRANSFORM_HPP
