#include "drawing_area.hpp"
#include <QMouseEvent>
#include <QPainter>
#include "drawing_context.hpp"
#include "pen_tool.hpp"
#include "select_tool.hpp"

namespace linjal {

drawing_area::drawing_area(model& model)
    : model_(model)
    , shape_(nullptr)
    , panning_(false)
    , image_visible_(true)
{
    setMouseTracking(true);
    use_pen_tool();
    model_.signal_shape_deleted().connect(sigc::mem_fun(this, &drawing_area::on_shape_deleted));
    model_.signal_color_changed().connect(sigc::mem_fun(this, &drawing_area::on_color_changed));
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

void drawing_area::set_image(QImage const& image)
{
    image_ = image;
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

QImage drawing_area::draw_to_image()
{
    QImage image(width(), height(), QImage::Format_RGB32);
    image.fill(Qt::black);

    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);

    drawing_context context(painter);
    model_.draw(context, camera_);

    return image;
}

void drawing_area::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    if (!image_.isNull() && image_visible_)
    {
        draw_image(painter);
    }

    drawing_context context(painter);
    model_.draw(context, camera_);
    tool_->on_draw(context);
}

void drawing_area::mousePressEvent(QMouseEvent* event)
{
    pointer_event pointer_event =
    {
        camera_.to_model_space({float(event->x()), float(event->y())}),
        event->modifiers()
    };

    if (event->button() == Qt::RightButton)
    {
        panning_ = true;
        grab_position_ = pointer_event.model_position;
    }
    else
    {
        tool_->on_button_press_event(pointer_event);
    }

    update();
}

void drawing_area::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::RightButton)
    {
        panning_ = false;
        return;
    }

    pointer_event pointer_event =
    {
        camera_.to_model_space({float(event->x()), float(event->y())}),
        event->modifiers()
    };

    tool_->on_button_release_event(pointer_event);
    update();
}

void drawing_area::mouseMoveEvent(QMouseEvent* event)
{
    math::vector2f event_position = {float(event->x()), float(event->y())};

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
            event->modifiers()
        };

        tool_->on_motion_notify_event(pointer_event);
    }

    update();
}

void drawing_area::wheelEvent(QWheelEvent* event)
{
    math::vector2f screen_position = {float(event->x()), float(event->y())};

    if (event->delta() > 0)
    {
        camera_.set_zoom(camera_.get_zoom() + 1, screen_position);
    }
    else if (event->delta() < 0)
    {
        int zoom = camera_.get_zoom();
        if (zoom > 1)
        {
            camera_.set_zoom(zoom - 1, screen_position);
        }
    }

    update();
}

void drawing_area::on_shape_deleted(shape* shape)
{
    if (shape_ == shape)
    {
        shape_ = nullptr;
    }
}

void drawing_area::on_color_changed(std::size_t row)
{
    update();
}

void drawing_area::draw_image(QPainter& painter) const
{
    auto position = camera_.to_screen_space({0.0f, 0.0f});
    auto size     = camera_.to_screen_scale({float(image_.width()), float(image_.height())});

    QRect rect(position.x(), position.y(), size.x(), size.y());
    painter.drawImage(rect, image_);
}

} // namespace linjal
