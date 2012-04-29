#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>
#include <memory>
#include "shape.hpp"
#include "tool.hpp"
#include "view_transform.hpp"

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
public:
    drawing_area();
    void new_shape();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();
    shape* pick(cml::vector2f const& position);

private:
    void delete_degenerate_shapes();
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);
    bool on_scroll_event(GdkEventScroll* event);

    std::vector<shape> shapes_;
    shape* shape_;
    view_transform transform_;
    std::unique_ptr<tool> tool_;

    friend class pen_tool;
    friend class select_tool;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
