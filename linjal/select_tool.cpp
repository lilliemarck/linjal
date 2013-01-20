#include "select_tool.hpp"
#include "drawing_area.hpp"

namespace linjal {

select_tool::select_tool(linjal::drawing_area* drawing_area) : tool(drawing_area)
{
}

void select_tool::on_delete()
{
    if (shape* shape = drawing_area_->shape_)
    {
        model_.delete_shape(*shape);
    }
}

void select_tool::on_draw(drawing_context& context)
{
    context.set_color({0, 0, 0, 255});
    if (drawing_area_->shape_)
    {
        for (auto const& node : drawing_area_->shape_->path)
        {
            context.circle(drawing_area_->camera_.to_screen_space(node.position), 2.0f);
            context.fill();
        }
    }
}

void select_tool::on_button_press_event(pointer_event const& event)
{
    drawing_area_->shape_ = model_.pick(event.model_position);
}

void select_tool::on_button_release_event(pointer_event const& event)
{
}

void select_tool::on_motion_notify_event(pointer_event const& event)
{
}

} // namespace linjal
