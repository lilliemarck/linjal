#include "drawing_area.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"

namespace linjal {

drawing_area::drawing_area(model& model)
    : model_(model)
    , shape_(nullptr)
    , panning_(false)
    , image_visible_(true)
{
    use_pen_tool();
    model_.signal_shape_deleted().connect(sigc::mem_fun(this, &drawing_area::on_shape_deleted));
    model_.signal_color_changed().connect(sigc::mem_fun(this, static_cast<void(QWidget::*)()>(&QWidget::update)));
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
    update();
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

void drawing_area::set_image_visible(bool visible)
{
    image_visible_ = visible;
}

void drawing_area::move_shape_up()
{
    if (shape_)
    {
        size_t new_index = model_.index_of_shape(*shape_) + 1;
        if (new_index < model_.shape_count())
        {
            shape_ = &model_.replace_shape(*shape_, new_index);
        }
        update();
    }
}

void drawing_area::move_shape_down()
{
    if (shape_)
    {
        size_t new_index = model_.index_of_shape(*shape_) - 1;
        if (new_index < model_.shape_count())
        {
            shape_ = &model_.replace_shape(*shape_, new_index);
        }
        update();
    }
}

Cairo::RefPtr<Cairo::ImageSurface> drawing_area::draw_to_image_surface()
{
    auto image = Cairo::ImageSurface::create(Cairo::Format::FORMAT_RGB24, width(), height());
    auto cairo = Cairo::Context::create(image);

    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();
    model_.draw(cairo, camera_);

    return image;
}

#if 0

bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();

    if (image_pattern_ && image_visible_)
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
    math::vector2f event_position = {float(event->x), float(event->y)};

    if (panning_)
    {
        auto offset = camera_.to_model_scale(event_position);
        camera_.set_position(math::vector2i(grab_position_ - offset));
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
    math::vector2f screen_position = {float(event->x), float(event->y)};

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

#endif

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
                                             position.x(), position.y()));

    cairo->set_source(image_pattern_);
    cairo->paint();
}

} // namespace linjal
