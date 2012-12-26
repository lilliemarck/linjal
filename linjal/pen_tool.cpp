#include "pen_tool.hpp"
#include "drawing_area.hpp"
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

    math::vector2f snap_position(float grid_size, math::vector2f const& position)
    {
        return {grid_size * std::round(position.x() / grid_size),
                grid_size * std::round(position.y() / grid_size)};
    }
} // namespace


pen_tool::pen_tool(linjal::drawing_area* drawing_area) :
    tool(drawing_area),
    state_(state::idle)
{
}

void pen_tool::on_delete()
{
    if (shape* shape = drawing_area_->shape_)
    {
        erase_points(shape->path, selection_);
    }

    selection_.clear();
    highlight_.clear();
    state_ = state::idle;
}

void pen_tool::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    if (!drawing_area_->shape_)
    {
        return;
    }

    auto const& camera = drawing_area_->camera_;
    shape const& shape = *drawing_area_->shape_;
    path const& path = shape.path;

    for (size_t i = 0; i < path.size(); ++i)
    {
        if (selection_.find(i) != end(selection_))
        {
            cairo->set_source_rgb(1.0, 0.0, 0.0);
        }
        else
        {
            cairo->set_source_rgb(0.0, 0.0, 0.0);
        }

        cairo_cirlce(cairo, camera.to_screen_space(path[i].position), 2.0f);
        cairo->fill();
        cairo->set_source_rgb(0.0, 0.0, 0.0);
        cairo_cirlce(cairo, camera.to_screen_space(path[i].control_point), 1.0f);
        cairo->fill();
    }
}

void pen_tool::on_button_press_event(pointer_event const& event)
{
    if (!drawing_area_->shape_)
    {
        return;
    }

    math::vector2f snapped = snap_position(0.5f, event.model_position);

    switch (highlight_.type())
    {
        case point_ref::null:
        {
            path& path = drawing_area_->shape_->path;
            size_t index = iterator_to_index(path, insert_point(path, snapped));
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

void pen_tool::on_button_release_event(pointer_event const& event)
{
    state_ = state::idle;
}

void pen_tool::on_motion_notify_event(pointer_event const& event)
{
    if (!drawing_area_->shape_)
    {
        drawing_area_->get_window()->set_cursor(Glib::RefPtr<Gdk::Cursor>());
        return;
    }

    path& path = drawing_area_->shape_->path;
    math::vector2f snapped = snap_position(0.5f, event.model_position);

    switch (state_)
    {
        case state::idle:
        {
            // Don't think it hurts to use the snapped position here
            float distance;
            highlight_ = nearest_point(path, snapped, distance);
            distance *= drawing_area_->camera_.get_zoom();

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
            math::vector2f move = snapped - drag_origin_;
            for (size_t index : selection_)
            {
                path[index].position += move;
            }
            drag_origin_ = snapped;
            break;
        }

        case state::drag_control_point:
        {
            path[*highlight_].control_point = snapped;
            break;
        }
    }
}

} // namespace linjal
