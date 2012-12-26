#ifndef LINJAL_CAMERA_HPP
#define LINJAL_CAMERA_HPP

#include <math/vector.hpp>

namespace linjal {

class camera
{
public:
    camera();
    math::vector2f to_model_space(math::vector2f const& position) const;
    math::vector2f to_model_scale(math::vector2f const& size) const;
    math::vector2f to_screen_space(math::vector2f const& position) const;
    math::vector2f to_screen_scale(math::vector2f const& size) const;

    math::vector2i get_position() const;
    void set_position(math::vector2i const& position);

    int get_zoom() const;
    void set_zoom(int zoom, math::vector2f const& cursor);

private:
    math::vector2i position_;
    int zoom_;
};

} // namespace linjal

#endif // LINJAL_CAMERA_HPP
