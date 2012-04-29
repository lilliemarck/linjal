#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>
#include <set>
#include "shape.hpp"

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
public:
    drawing_area();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();

private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);

    std::vector<shape> shapes_;
    shape* shape_;
    bool highlighting_;
    size_t highlighted_point_;
    std::set<size_t> selection_;
    cml::vector2f drag_origin_;
    bool dragging_;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
