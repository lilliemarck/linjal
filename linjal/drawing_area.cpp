#include "drawing_area.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"

namespace linjal {

namespace
{
    Cairo::RefPtr<Cairo::Context> create_null_cairo_context()
    {
        return Cairo::Context::create(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 0, 0));
    }

    void curve(Cairo::RefPtr<Cairo::Context> const& cairo, shape const& shape)
    {
        for (auto const& node : shape)
        {
            cairo->line_to(node.position[0], node.position[1]);
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
    shape* picked_shape = nullptr;

    for (auto& shape : shapes_)
    {
        curve(cairo, shape);

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

    for (auto iter = begin(shapes_); iter != end(shapes_);)
    {
        auto const& shape = *iter;

        curve(cairo, shape);

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
        curve(cairo, shape);
        cairo->fill();
    }

    tool_->on_draw(cairo);
    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    tool_->on_button_press_event(*event);
    queue_draw();
    return true;
}

bool drawing_area::on_button_release_event(GdkEventButton* event)
{
    tool_->on_button_release_event(*event);
    queue_draw();
    return true;
}

bool drawing_area::on_motion_notify_event(GdkEventMotion* event)
{
    tool_->on_motion_notify_event(*event);
    queue_draw();
    return true;
}

} // namespace linjal
