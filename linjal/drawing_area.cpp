#include "drawing_area.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"

namespace linjal {

drawing_area::drawing_area(model& model) :
    model_(model),
    shape_(nullptr),
    panning_(false)
{
    use_pen_tool();
    add_events(Gdk::POINTER_MOTION_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
    model_.signal_shape_deleted().connect(sigc::mem_fun(this, &drawing_area::on_shape_deleted));
    model_.signal_color_changed().connect(sigc::mem_fun(this, &drawing_area::queue_draw));
}

void drawing_area::new_shape()
{
    shape_ = &model_.new_shape();
    // Create a new tool so that selections are cleared. Don't call
    // use_pen_tool because it calls delete_degenerate_shapes!
    tool_ = std::unique_ptr<tool>(new pen_tool(this));
}

void drawing_area::use_pen_tool()
{
    model_.delete_degenerate_shapes();
    tool_ = std::unique_ptr<tool>(new pen_tool(this));
}

void drawing_area::use_select_tool()
{
    model_.delete_degenerate_shapes();
    tool_ = std::unique_ptr<tool>(new select_tool(this));
}

void drawing_area::delete_selection()
{
    tool_->on_delete();
    queue_draw();
}

shape* drawing_area::selected_shape()
{
    return shape_;
}

void drawing_area::set_image(Cairo::RefPtr<Cairo::ImageSurface> const& image)
{
    image_pattern_ = Cairo::SurfacePattern::create(image);
    image_pattern_->set_filter(Cairo::FILTER_NEAREST);
}

bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();

    if (image_pattern_)
    {
        draw_image(cairo);
    }

    model_.draw(cairo, camera_);
    tool_->on_draw(cairo);
    return true;
}

bool drawing_area::on_button_press_event(GdkEventButton* event)
{
    pointer_event pointer_event =
    {
        camera_.to_model_space({float(event->x), float(event->y)}),
        event->state
    };

    if (event->button == 3)
    {
        panning_ = true;
        grab_position_ = pointer_event.model_position;
    }
    else
    {
        tool_->on_button_press_event(pointer_event);
    }

    queue_draw();
    return true;
}

bool drawing_area::on_button_release_event(GdkEventButton* event)
{
    if (event->button == 3)
    {
        panning_ = false;
        return true;
    }

    pointer_event pointer_event =
    {
        camera_.to_model_space({float(event->x), float(event->y)}),
        event->state
    };

    tool_->on_button_release_event(pointer_event);
    queue_draw();
    return true;
}

bool drawing_area::on_motion_notify_event(GdkEventMotion* event)
{
    cml::vector2f event_position = {float(event->x), float(event->y)};

    if (panning_)
    {
        auto offset = camera_.to_model_scale(event_position);
        camera_.set_position(grab_position_ - offset);
    }
    else
    {
        pointer_event pointer_event =
        {
            camera_.to_model_space(event_position),
            event->state
        };

        tool_->on_motion_notify_event(pointer_event);
    }

    queue_draw();
    return true;
}

bool drawing_area::on_scroll_event(GdkEventScroll* event)
{
    cml::vector2f screen_position = {float(event->x), float(event->y)};

    if (event->direction == GDK_SCROLL_UP)
    {
        camera_.set_zoom(camera_.get_zoom() + 1, screen_position);
    }
    else if (event->direction == GDK_SCROLL_DOWN)
    {
        int zoom = camera_.get_zoom();
        if (zoom > 1)
        {
            camera_.set_zoom(zoom - 1, screen_position);
        }
    }

    queue_draw();
    return true;
}

void drawing_area::on_shape_deleted(shape* shape)
{
    if (shape_ == shape)
    {
        shape_ = nullptr;
    }
}

void drawing_area::draw_image(Cairo::RefPtr<Cairo::Context> const& cairo) const
{
    auto position = camera_.get_position();
    auto inv_zoom = 1.0 / camera_.get_zoom();

    image_pattern_->set_matrix(Cairo::Matrix(inv_zoom, 0.0,
                                             0.0, inv_zoom,
                                             position[0], position[1]));

    cairo->set_source(image_pattern_);
    cairo->paint();
}

} // namespace linjal
