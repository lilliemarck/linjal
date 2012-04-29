#include "drawing_area.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"
#include "utils.hpp"

namespace linjal {

namespace
{
    Cairo::RefPtr<Cairo::Context> create_null_cairo_context()
    {
        return Cairo::Context::create(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 0, 0));
    }

    template <typename Container, typename Iterator>
    shape::const_iterator wraparound_next(Container const& container, Iterator iterator)
    {
        if (++iterator != end(container))
        {
            return iterator;
        }
        else
        {
            return begin(container);
        }
    }

    void curve(Cairo::RefPtr<Cairo::Context> const& cairo, shape const& shape, view_transform const& transform)
    {
        if (shape.empty())
        {
            return;
        }

        const float k = 0.551784f;
        cairo_move_to(cairo, transform.to_screen(shape.front().position));

        for (shape::const_iterator iter = begin(shape); iter != end(shape); ++iter)
        {
            auto const& node = *iter;
            auto const& next = *wraparound_next(shape, iter);
            auto b = lerp(node.position, node.control_point, k);
            auto c = lerp(next.position, node.control_point, k);
            cairo_curve_to(cairo,
                           transform.to_screen(b),
                           transform.to_screen(c),
                           transform.to_screen(next.position));
        }
    }
} // namespace

drawing_area::drawing_area() : shape_(nullptr)
{
    use_pen_tool();
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

void drawing_area::new_shape()
{
    shapes_.emplace_back();
    shape_ = &shapes_.back();

    // Create a new tool so that selections are cleared. Don't call
    // use_pen_tool because it calls delete_degenerate_shapes!
    tool_ = std::unique_ptr<tool>(new pen_tool(this));
}

void drawing_area::use_pen_tool()
{
    delete_degenerate_shapes();
    tool_ = std::unique_ptr<tool>(new pen_tool(this));
}

void drawing_area::use_select_tool()
{
    delete_degenerate_shapes();
    tool_ = std::unique_ptr<tool>(new select_tool(this));
}

void drawing_area::delete_selection()
{
    tool_->on_delete();
    queue_draw();
}

shape* drawing_area::pick(cml::vector2f const& position)
{
    auto cairo = create_null_cairo_context();
    view_transform no_transform;
    shape* picked_shape = nullptr;

    for (auto& shape : shapes_)
    {
        curve(cairo, shape, no_transform);

        if (cairo->in_fill(position[0], position[1]))
        {
            picked_shape = &shape;
        }

        cairo->fill();
    }

    return picked_shape;
}

void drawing_area::delete_degenerate_shapes()
{
    auto cairo = create_null_cairo_context();
    view_transform no_transform;

    for (auto iter = begin(shapes_); iter != end(shapes_);)
    {
        auto const& shape = *iter;

        curve(cairo, shape, no_transform);

        double x1, y1, x2, y2;
        cairo->get_fill_extents(x1, y1, x2, y2);
        cairo->fill();

        if (x1 == 0.0 && y1 == 0.0 && x2 == 0.0 && y2 == 0.0)
        {
            iter = shapes_.erase(iter);

            if (shape_ == &shape)
            {
                shape_ = nullptr;
            }
        }
        else
        {
            ++iter;
        }
    }
}

bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();
    cairo->set_source_rgb(0.3, 0.4, 0.7);

    for (auto const& shape : shapes_)
    {
        curve(cairo, shape, transform_);
        cairo->fill();
    }

    tool_->on_draw(cairo);
    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    pointer_event pointer_event =
    {
        transform_.to_model({float(event->x), float(event->y)}),
        event->state
    };

    tool_->on_button_press_event(pointer_event);
    queue_draw();
    return true;
}

bool drawing_area::on_button_release_event(GdkEventButton* event)
{
    pointer_event pointer_event =
    {
        transform_.to_model({float(event->x), float(event->y)}),
        event->state
    };

    tool_->on_button_release_event(pointer_event);
    queue_draw();
    return true;
}

bool drawing_area::on_motion_notify_event(GdkEventMotion* event)
{
    pointer_event pointer_event =
    {
        transform_.to_model({float(event->x), float(event->y)}),
        event->state
    };

    tool_->on_motion_notify_event(pointer_event);
    queue_draw();
    return true;
}

bool drawing_area::on_scroll_event(GdkEventScroll* event)
{
    if (event->direction == GDK_SCROLL_UP)
    {
        transform_.zoom_in();
    }
    // Zoom out
    else if (event->direction == GDK_SCROLL_DOWN)
    {
        transform_.zoom_out();
    }

    queue_draw();
    return true;
}

} // namespace linjal
