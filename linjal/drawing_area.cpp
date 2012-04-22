#include "drawing_area.hpp"

namespace linjal {

drawing_area::drawing_area()
{
    add_events(Gdk::BUTTON_PRESS_MASK);
}


bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();
    cairo->set_source_rgb(0.0, 0.0, 0.0);

    for (auto const& point : shape_)
    {
        cairo->line_to(point[0], point[1]);
    }

    cairo->close_path();
    cairo->set_line_width(1.0);
    cairo->stroke();
    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    shape_.emplace_back<>(event->x, event->y);
    queue_draw();
    return true;
}

} // namespace linjal
