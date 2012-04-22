#include "drawing_area.hpp"
#include "math.hpp"

namespace linjal {

namespace
{
    void cirlce(Cairo::RefPtr<Cairo::Context> const& cairo,
                cml::vector2f const& center,
                float radius)
    {
        cairo->arc(center[0], center[1], radius, 0.0, cml::constantsd::two_pi());
    }
} // namespace

drawing_area::drawing_area() :
    higlighting_(false),
    has_selection_(false),
    dragging_(false)
{
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
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

    if (has_selection_)
    {
        cirlce(cairo, shape_[selected_point_], 2.0f);
        cairo->fill();
    }

    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    if (higlighting_)
    {
        has_selection_ = true;
        selected_point_ = higlighted_point_;
    }
    else
    {
        cml::vector2f point = {float(event->x), float(event->y)};
        selected_point_ = std::distance(shape_.begin(), insert_point(shape_, point));
        higlighted_point_ = selected_point_;
        get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
    }

    dragging_ = true;
    queue_draw();
    return true;
}

bool drawing_area::on_button_release_event(GdkEventButton* event)
{
    dragging_ = false;
    return true;
}

bool drawing_area::on_motion_notify_event(GdkEventMotion* event)
{
    if (dragging_)
    {
        shape_[selected_point_].set(event->x, event->y);
    }
    else
    {
        cml::vector2f point = {float(event->x), float(event->y)};
        shape::iterator iter = nearest_point(shape_, point);

        if (iter != shape_.end() && distance(point, *iter) < 5.0)
        {
            higlighting_ = true;
            higlighted_point_ = std::distance(shape_.begin(), iter);
            get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
        }
        else
        {
            higlighting_ = false;
            get_window()->set_cursor(Gdk::Cursor::create(Gdk::PENCIL));
        }
    }

    queue_draw();
    return false;
}

} // namespace linjal
