#include "pen_tool.hpp"
#include "drawing_area.hpp"
#include "math.hpp"
#include "utils.hpp"

namespace linjal {

namespace
{
    void toggle_selection(std::set<size_t>& selection, size_t index)
    {
        auto iter = selection.find(index);

        if (iter == selection.end())
        {
            selection.insert(index);
        }
        else
        {
            selection.erase(iter);
        }
    }

    template <typename Container>
    size_t iterator_to_index(Container const& container, typename Container::const_iterator iter)
    {
        return std::distance(container.begin(), iter);
    }

    cml::vector2f snap_position(float grid_size, cml::vector2f const& position)
    {
        return {grid_size * std::round(position[0] / grid_size),
                grid_size * std::round(position[1] / grid_size)};
    }
} // namespace


pen_tool::pen_tool(linjal::drawing_area* drawing_area) :
    tool(drawing_area),
    state_(state::idle)
{
}

void pen_tool::on_delete()
{
    for (auto iter = selection_.rbegin(); iter != selection_.rend(); ++iter)
    {
        drawing_area_->shape_->erase(drawing_area_->shape_->begin() + *iter);
    }

    selection_.clear();
    highlight_.clear();
    state_ = state::idle;
}

void pen_tool::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(0.0, 0.0, 0.0);
    for (size_t index : selection_)
    {
        node const& node = (*drawing_area_->shape_)[index];
        cairo_cirlce(cairo, node.position, 2.0f);
        cairo->fill();
        cairo_cirlce(cairo, node.control_point, 1.0f);
        cairo->fill();
    }
}

void pen_tool::on_button_press_event(GdkEventButton const& event)
{
    if (!drawing_area_->shape_)
    {
        return;
    }

    cml::vector2f snapped = snap_position(5.0f, {float(event.x), float(event.y)});

    switch (highlight_.type())
    {
        case point_ref::null:
        {
            shape& shape = *drawing_area_->shape_;
            size_t index = iterator_to_index(shape, insert_point(shape, snapped));
            highlight_.set_control_point(index);
            selection_.clear();
            state_ = state::drag_control_point;
            drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
            break;
        }

        case point_ref::position:
        {
            if (event.state & Gdk::CONTROL_MASK)
            {
                toggle_selection(selection_, *highlight_);
            }
            else if (selection_.find(*highlight_) == selection_.end())
            {
                selection_.clear();
                selection_.insert(*highlight_);
            }

            state_ = state::drag_selection;
            drag_origin_ = snapped;
            break;
        }

        case point_ref::control_point:
        {
            selection_.clear();
            state_ = state::drag_control_point;
            break;
        }
    }
}

void pen_tool::on_button_release_event(GdkEventButton const& event)
{
    state_ = state::idle;
}

void pen_tool::on_motion_notify_event(GdkEventMotion const& event)
{
    if (!drawing_area_->shape_)
    {
        drawing_area_->get_window()->set_cursor(Glib::RefPtr<Gdk::Cursor>());
        return;
    }

    shape& shape = *drawing_area_->shape_;
    cml::vector2f snapped = snap_position(5.0f, {float(event.x), float(event.y)});

    switch (state_)
    {
        case state::idle:
        {
            // Don't think it hurts to use the snapped position here
            float distance;
            highlight_ = nearest_point(shape, snapped, distance);

            if (highlight_.type() == point_ref::position && distance < 5.0)
            {
                drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
            }
            else if (highlight_.type() == point_ref::control_point && distance < 5.0)
            {
                drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::CROSSHAIR));
            }
            else
            {
                highlight_.clear();
                drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::PENCIL));
            }
            break;
        }

        case state::drag_selection:
        {
            cml::vector2f move = snapped - drag_origin_;
            for (size_t index : selection_)
            {
                shape[index].position += move;
            }
            drag_origin_ = snapped;
            break;
        }

        case state::drag_control_point:
        {
            shape[*highlight_].control_point = snapped;
            break;
        }
    }
}

} // namespace linjal
