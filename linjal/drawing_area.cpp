#include "drawing_area.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"

namespace linjal {

drawing_area::drawing_area()
{
    new_shape();
    use_pen_tool();
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
}

void drawing_area::new_shape()
{
    shapes_.emplace_back();
    shape_ = &shapes_.back();
    use_pen_tool(); // Create a new tool so that selections are cleared
}

void drawing_area::use_pen_tool()
{
    tool_ = std::unique_ptr<tool>(new pen_tool(this));
}

void drawing_area::use_select_tool()
{
    tool_ = std::unique_ptr<tool>(new select_tool(this));
}

void drawing_area::delete_selection()
{
    tool_->on_delete();
    queue_draw();
}

shape* drawing_area::pick(cml::vector2f const& position)
{
    auto surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 0, 0);
    auto cairo = Cairo::Context::create(surface);
    shape* picked_shape = nullptr;

    for (auto& shape : shapes_)
    {
        for (auto const& point : shape)
        {
            cairo->line_to(point[0], point[1]);
        }

        if (cairo->in_fill(position[0], position[1]))
        {
            picked_shape = &shape;
        }

        cairo->fill();
    }

    return picked_shape;
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

        cairo->close_path();
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
