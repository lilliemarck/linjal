#include "select_tool.hpp"
#include "drawing_area.hpp"
#include "utils.hpp"

namespace linjal {

select_tool::select_tool(linjal::drawing_area* drawing_area) : tool(drawing_area)
{
}

void select_tool::on_delete()
{
    for (auto iter = begin(drawing_area_->shapes_); iter != end(drawing_area_->shapes_); ++iter)
    {
        if (&*iter == drawing_area_->shape_)
        {
            drawing_area_->shapes_.erase(iter);
            drawing_area_->shape_ = nullptr;
            return;
        }
    }
}

void select_tool::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(0.0, 0.0, 0.0);
    if (drawing_area_->shape_)
    {
        for (auto const& node : *drawing_area_->shape_)
        {
            cairo_cirlce(cairo, drawing_area_->transform_.to_screen(node.position), 2.0f);
            cairo->fill();
        }
    }
}

void select_tool::on_button_press_event(pointer_event const& event)
{
    drawing_area_->shape_ = drawing_area_->pick(event.model_position);
}

void select_tool::on_button_release_event(pointer_event const& event)
{
}

void select_tool::on_motion_notify_event(pointer_event const& event)
{
}

} // namespace linjal
