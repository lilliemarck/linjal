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
    drawing_area();
    void new_shape();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();

private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);
    bool on_scroll_event(GdkEventScroll* event);
    void on_shape_deleted(shape* shape);

    model model_;
    shape* shape_;
    camera camera_;
    std::unique_ptr<tool> tool_;
    bool panning_;
    cml::vector2f grab_position_;

    friend class tool;
    friend class pen_tool;
    friend class select_tool;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
