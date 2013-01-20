#pragma once

#include <math/vector.hpp>
#include <Qt>

namespace linjal {

class drawing_area;
class drawing_context;
class model;

struct pointer_event
{
    math::vector2f model_position;
    Qt::KeyboardModifiers modifiers;
};

class tool
{
public:
    tool(linjal::drawing_area* drawing_area);

    virtual ~tool() {}
    virtual void on_delete() {}
    virtual void on_draw(drawing_context& context) {}
    virtual void on_button_press_event(pointer_event const& event) {}
    virtual void on_button_release_event(pointer_event const& event) {}
    virtual void on_motion_notify_event(pointer_event const& event) {}

protected:
    drawing_area* drawing_area_;
    model& model_;
};

} // namespace linjal
