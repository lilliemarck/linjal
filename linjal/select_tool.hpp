#ifndef LINJAL_SELECT_TOOL_HPP
#define LINJAL_SELECT_TOOL_HPP

#include "tool.hpp"

namespace linjal {

class select_tool : public tool
{
public:
    select_tool(linjal::drawing_area* drawing_area);
    void on_delete() override;
    void on_draw(Cairo::RefPtr<Cairo::Context> const& cairo) override;
    void on_button_press_event(pointer_event const& event) override;
    void on_button_release_event(pointer_event const& event) override;
    void on_motion_notify_event(pointer_event const& event) override;

private:
};

} // namespace linjal

#endif // LINJAL_SELECT_TOOL_HPP
