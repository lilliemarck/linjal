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

    void toggle_selection(std::set<size_t>& selection, size_t index)
    {
        auto iter = selection.find(index);

        if (iter == selection.end())
        {
            selection.insert(index);
        }
        else
        {
            selection.erase(iter);
        }
    }

    template <typename Container>
    size_t iterator_to_index(Container const& container, typename Container::const_iterator iter)
    {
        return std::distance(container.begin(), iter);
    }
} // namespace

drawing_area::drawing_area() :
    higlighting_(false),
    dragging_(false)
{
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

void drawing_area::delete_selection()
{
    for (auto iter = selection_.rbegin(); iter != selection_.rend(); ++iter)
    {
        shape_.erase(shape_.begin() + *iter);
    }

    selection_.clear();
    higlighting_ = false;
    dragging_ = false;

    queue_draw();
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

    for (size_t index : selection_)
    {
        cirlce(cairo, shape_[index], 2.0f);
        cairo->fill();
    }

    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    if (higlighting_)
    {
        if (event->state & Gdk::CONTROL_MASK)
        {
            toggle_selection(selection_, higlighted_point_);
        }
        else if (selection_.find(higlighted_point_) == selection_.end())
        {
            selection_.clear();
            selection_.insert(higlighted_point_);
        }
    }
    else
    {
        cml::vector2f point = {float(event->x), float(event->y)};
        higlighted_point_ = iterator_to_index(shape_, insert_point(shape_, point));
        selection_.clear();
        selection_.insert(higlighted_point_);
        get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
    }

    drag_origin_.set(event->x, event->y);
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
        cml::vector2d posision = {event->x, event->y};
        cml::vector2d move = posision - drag_origin_;
        for (size_t index : selection_)
        {
            shape_[index] += move;
        }
        drag_origin_ = posision;
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
