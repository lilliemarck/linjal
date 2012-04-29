#ifndef LINJAL_PEN_TOOL_HPP
#define LINJAL_PEN_TOOL_HPP

#include <cml/cml.h>
#include <set>
#include "tool.hpp"

namespace linjal {

class pen_tool : public tool
{
public:
    pen_tool(linjal::drawing_area* drawing_area);
    virtual void on_delete();
    virtual void on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    virtual void on_button_press_event(GdkEventButton const& event);
    virtual void on_button_release_event(GdkEventButton const& event);
    virtual void on_motion_notify_event(GdkEventMotion const& event);

private:
    bool highlighting_;
    size_t highlighted_point_;
    std::set<size_t> selection_;
    cml::vector2f drag_origin_;
    bool dragging_;
};

} // namespace linjal

#endif // LINJAL_PEN_TOOL_HPP
