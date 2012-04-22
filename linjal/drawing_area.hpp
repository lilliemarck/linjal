#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>
#include "shape.hpp"

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
public:
    drawing_area();

private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo) override;
    bool on_button_press_event(GdkEventButton* event) override;
    bool on_button_release_event(GdkEventButton* event) override;
    bool on_motion_notify_event(GdkEventMotion* event) override;

    shape shape_;
    bool dragging_;
    size_t selection_;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
