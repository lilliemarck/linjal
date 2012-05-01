#ifndef LINJAL_CAMERA_HPP
#define LINJAL_CAMERA_HPP

#include <cml/cml.h>

namespace linjal {

class camera
{
public:
    camera();
    cml::vector2f to_model_space(cml::vector2f const& position) const;
    cml::vector2f to_model_scale(cml::vector2f const& size) const;
    cml::vector2f to_screen_space(cml::vector2f const& position) const;

    cml::vector2f get_position();
    void set_position(cml::vector2f const& position);

    int get_zoom() const;
    void set_zoom(int zoom, cml::vector2f const& cursor);

private:
    cml::vector2f position_;
    int zoom_;
};

} // namespace linjal

#endif // LINJAL_CAMERA_HPP
