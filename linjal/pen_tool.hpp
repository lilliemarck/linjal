#pragma once

#include <math/vector.hpp>
#include <set>
#include "shape.hpp"
#include "tool.hpp"

namespace linjal {

class pen_tool : public tool
{
public:
    pen_tool(linjal::drawing_area* drawing_area);
    void on_delete() override;
    void on_draw(drawing_context& context) override;
    void on_button_press_event(pointer_event const& event) override;
    void on_button_release_event(pointer_event const& event) override;
    void on_motion_notify_event(pointer_event const& event) override;

private:
    enum class state
    {
        idle,
        drag_control_point,
        drag_selection
    };

    point_ref highlight_;
    std::set<size_t> selection_;
    state state_;
    math::vector2f drag_origin_;
};

} // namespace linjal
