#pragma once

#include "tool.hpp"

namespace linjal {

class select_tool : public tool
{
public:
    select_tool(linjal::drawing_area* drawing_area);
    void on_delete() override;
    void on_draw(drawing_context& context) override;
    void on_button_press_event(pointer_event const& event) override;
    void on_button_release_event(pointer_event const& event) override;
    void on_motion_notify_event(pointer_event const& event) override;

private:
};

} // namespace linjal
