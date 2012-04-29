#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>
#include <memory>
#include "shape.hpp"
#include "tool.hpp"

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
public:
    drawing_area();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();
    shape* pick(cml::vector2f const& position);

private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    bool on_button_press_event(GdkEventButton* event);
    bool on_button_release_event(GdkEventButton* event);
    bool on_motion_notify_event(GdkEventMotion* event);

    std::vector<shape> shapes_;
    shape* shape_;
    std::unique_ptr<tool> tool_;

    friend class pen_tool;
    friend class select_tool;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
