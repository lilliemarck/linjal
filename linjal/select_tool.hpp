#ifndef LINJAL_SELECT_TOOL_HPP
#define LINJAL_SELECT_TOOL_HPP

#include "tool.hpp"

namespace linjal {

class select_tool : public tool
{
public:
    select_tool(linjal::drawing_area* drawing_area);
    virtual void on_delete();
    virtual void on_draw(Cairo::RefPtr<Cairo::Context> const& cairo);
    virtual void on_button_press_event(GdkEventButton const& event);
    virtual void on_button_release_event(GdkEventButton const& event);
    virtual void on_motion_notify_event(GdkEventMotion const& event);

private:
};

} // namespace linjal

#endif // LINJAL_SELECT_TOOL_HPP
