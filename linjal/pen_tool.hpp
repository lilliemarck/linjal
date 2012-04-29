#ifndef LINJAL_PEN_TOOL_HPP
#define LINJAL_PEN_TOOL_HPP

#include <cml/cml.h>
#include <set>
#include "shape.hpp"
#include "tool.hpp"

namespace linjal {

class pen_tool : public tool
{
public:
    pen_tool(linjal::drawing_area* drawing_area);
    virtual void on_delete();
    virtual void on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    virtual void on_button_press_event(pointer_event const& event);
    virtual void on_button_release_event(pointer_event const& event);
    virtual void on_motion_notify_event(pointer_event const& event);

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
    cml::vector2f drag_origin_;
};

} // namespace linjal

#endif // LINJAL_PEN_TOOL_HPP
