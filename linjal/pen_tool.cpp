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
    highlighting_(false),
    dragging_(false)
{
}

void pen_tool::on_delete()
{
    for (auto iter = selection_.rbegin(); iter != selection_.rend(); ++iter)
    {
        drawing_area_->shape_->erase(drawing_area_->shape_->begin() + *iter);
    }

    selection_.clear();
    highlighting_ = false;
    dragging_ = false;
}

void pen_tool::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(0.0, 0.0, 0.0);
    for (size_t index : selection_)
    {
        cairo_cirlce(cairo, (*drawing_area_->shape_)[index], 2.0f);
        cairo->fill();
    }
}

void pen_tool::on_button_press_event(GdkEventButton const& event)
{
    cml::vector2f snapped = snap_position(5.0f, {float(event.x), float(event.y)});

    if (highlighting_)
    {
        if (event.state & Gdk::CONTROL_MASK)
        {
            toggle_selection(selection_, highlighted_point_);
        }
        else if (selection_.find(highlighted_point_) == selection_.end())
        {
            selection_.clear();
            selection_.insert(highlighted_point_);
        }
    }
    else if (drawing_area_->shape_)
    {
        shape& shape = *drawing_area_->shape_;
        highlighted_point_ = iterator_to_index(shape, insert_point(shape, snapped));
        selection_.clear();
        selection_.insert(highlighted_point_);
        drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
    }

    drag_origin_ = snapped;
    dragging_ = true;
}

void pen_tool::on_button_release_event(GdkEventButton const& event)
{
    dragging_ = false;
}

void pen_tool::on_motion_notify_event(GdkEventMotion const& event)
{
    cml::vector2f snapped = snap_position(5.0f, {float(event.x), float(event.y)});

    if (dragging_)
    {
        shape& shape = *drawing_area_->shape_;
        cml::vector2f move = snapped - drag_origin_;
        for (size_t index : selection_)
        {
            shape[index] += move;
        }
        drag_origin_ = snapped;
    }
    else if (drawing_area_->shape_)
    {
        shape& shape = *drawing_area_->shape_;
        shape::iterator iter = nearest_point(shape, snapped);

        if (iter != shape.end() && distance(snapped, *iter) < 5.0)
        {
            highlighting_ = true;
            highlighted_point_ =  std::distance(shape.begin(), iter);
            drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::HAND2));
        }
        else
        {
            highlighting_ = false;
            drawing_area_->get_window()->set_cursor(Gdk::Cursor::create(Gdk::PENCIL));
        }
    }
}

} // namespace linjal
