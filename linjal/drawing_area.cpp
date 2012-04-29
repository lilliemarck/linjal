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

    cml::vector2f snap_position(float grid_size, cml::vector2f const& position)
    {
        return {grid_size * std::round(position[0] / grid_size),
                grid_size * std::round(position[1] / grid_size)};
    }
} // namespace

drawing_area::drawing_area() :
    highlighting_(false),
    dragging_(false)
{
    shapes_.emplace_back();
    shape_ = &shapes_.front();

    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

void drawing_area::delete_selection()
{
    for (auto iter = selection_.rbegin(); iter != selection_.rend(); ++iter)
    {
        shape_->erase(shape_->begin() + *iter);
    }

    selection_.clear();
    highlighting_ = false;
    dragging_ = false;

    queue_draw();
}

bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();
    cairo->set_source_rgb(0.3, 0.4, 0.7);

    for (auto const& shape : shapes_)
    {
        for (auto const& point : shape)
        {
            cairo->line_to(point[0], point[1]);
        }
    }

    cairo->close_path();
    cairo->fill();
    cairo->set_source_rgb(0.0, 0.0, 0.0);

    for (size_t index : selection_)
    {
        cirlce(cairo, (*shape_)[index], 2.0f);
        cairo->fill();
    }

    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    cml::vector2f snapped = snap_position(5.0f, {float(event->x), float(event->y)});

    if (highlighting_)
    {
        if (event->state & Gdk::CONTROL_MASK)
        {
            toggle_selection(selection_, highlighted_point_);
        }
        else if (selection_.find(highlighted_point_) == selection_.end())
        {
            selection_.clear();
            selection_.insert(highlighted_point_);
        }
    }
    else if (shape_)
    {
        highlighted_point_ = iterator_to_index(*shape_, insert_point(*shape_, snapped));
        selection_.clear();
        selection_.insert(highlighted_point_);
        get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
    }

    drag_origin_ = snapped;
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
    cml::vector2f snapped = snap_position(5.0f, {float(event->x), float(event->y)});

    if (dragging_)
    {
        cml::vector2f move = snapped - drag_origin_;
        for (size_t index : selection_)
        {
            (*shape_)[index] += move;
        }
        drag_origin_ = snapped;
    }
    else if (shape_)
    {
        shape::iterator iter = nearest_point(*shape_, snapped);

        if (iter != shape_->end() && distance(snapped, *iter) < 5.0)
        {
            highlighting_ = true;
            highlighted_point_ =  std::distance(shape_->begin(), iter);
            get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
        }
        else
        {
            highlighting_ = false;
            get_window()->set_cursor(Gdk::Cursor::create(Gdk::PENCIL));
        }
    }

    queue_draw();
    return false;
}

} // namespace linjal
