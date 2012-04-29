#ifndef LINJAL_VIEW_TRANSFORM_HPP
#define LINJAL_VIEW_TRANSFORM_HPP

#include <cml/cml.h>

namespace linjal {

class view_transform
{
public:
    view_transform();
    cml::vector2f to_model(cml::vector2f const& screen) const;
    cml::vector2f to_screen(cml::vector2f const& model) const;
    void zoom_in();
    void zoom_out();
    int zoom() const;

private:
    int zoom_;
};

} // namespace linjal

#endif // LINJAL_VIEW_TRANSFORM_HPP
