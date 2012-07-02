#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>
#include <memory>
#include "camera.hpp"
#include "model.hpp"
#include "shape.hpp"
#include "tool.hpp"

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
public:
    drawing_area(model &model);
    void new_shape();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();
    shape* selected_shape();
    void set_image(Cairo::RefPtr<Cairo::ImageSurface> const& image);
    void set_image_visible(bool visible);
    void move_shape_up();
    void move_shape_down();
    Cairo::RefPtr<Cairo::ImageSurface> draw_to_image_surface();

private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);
    bool on_scroll_event(GdkEventScroll* event);
    void on_shape_deleted(shape* shape);
    void draw_image(Cairo::RefPtr<Cairo::Context> const& cairo) const;

    model& model_;
    shape* shape_;
    camera camera_;
    std::unique_ptr<tool> tool_;
    bool panning_;
    cml::vector2f grab_position_;
    Cairo::RefPtr<Cairo::SurfacePattern> image_pattern_;
    bool image_visible_;

    friend class tool;
    friend class pen_tool;
    friend class select_tool;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
