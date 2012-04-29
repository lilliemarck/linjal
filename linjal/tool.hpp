#ifndef LINJAL_TOOL_HPP
#define LINJAL_TOOL_HPP

#include <gdkmm.h>

namespace linjal {

class drawing_area;

class tool
{
public:
    tool(linjal::drawing_area* drawing_area);

    virtual ~tool() {}
    virtual void on_delete() {}
    virtual void on_draw(Cairo::RefPtr<Cairo::Context> const& cairo) {}
    virtual void on_button_press_event(GdkEventButton const& event) {}
    virtual void on_button_release_event(GdkEventButton const& event) {}
    virtual void on_motion_notify_event(GdkEventMotion const& event) {}

protected:
    drawing_area* drawing_area_;
};

} // namespace linjal

#endif // LINJAL_TOOL_HPP
