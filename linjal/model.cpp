#include "model.hpp"
#include "camera.hpp"
#include "shape.hpp"

#include <cairomm/cairomm.h>

namespace linjal {

namespace
{
    Cairo::RefPtr<Cairo::Context> create_null_cairo_context()
    {
        return Cairo::Context::create(Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, 0, 0));
    }
} // namespace

model::model()
{
    colors_.push_back({"red", {255, 0, 0, 255}});
    colors_.push_back({"green", {0, 255, 0, 255}});
}

shape& model::new_shape()
{
    shapes_.emplace_back();
    return shapes_.back();
}

void model::delete_shape(shape& shape)
{
    for (auto iter = begin(shapes_); iter != end(shapes_); ++iter)
    {
        if (&*iter == &shape)
        {
            shapes_.erase(iter);
            shape_deleted_(&shape);
            return;
        }
    }
}

void model::delete_degenerate_shapes()
{
    auto cairo = create_null_cairo_context();
    camera no_transform;

    for (auto iter = begin(shapes_); iter != end(shapes_);)
    {
        auto& shape = *iter;
        shape_curve(shape, cairo, no_transform);

        double x1, y1, x2, y2;
        cairo->get_fill_extents(x1, y1, x2, y2);
        cairo->fill();

        if (x1 == 0.0 && y1 == 0.0 && x2 == 0.0 && y2 == 0.0)
        {
            iter = shapes_.erase(iter);
            shape_deleted_(&shape);
        }
        else
        {
            ++iter;
        }
    }
}

shape* model::pick(cml::vector2f const& position)
{
    auto cairo = create_null_cairo_context();
    camera no_transform;
    shape* picked_shape = nullptr;

    for (auto& shape : shapes_)
    {
        shape_curve(shape, cairo, no_transform);

        if (cairo->in_fill(position[0], position[1]))
        {
            picked_shape = &shape;
        }

        cairo->fill();
    };

    return picked_shape;
}

void model::draw(Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera)
{
    cairo->set_source_rgb(0.3, 0.4, 0.7);

    for (auto const& shape : shapes_)
    {
        shape_curve(shape, cairo, camera);
        cairo->fill();
    }
}

size_t model::new_color()
{
    size_t index  = colors_.size();
    colors_.push_back({"color", {0, 0, 255, 255}});
    return index;
}

void model::delete_color(size_t index)
{
    colors_.erase(colors_.begin() + index);
}

size_t model::color_count() const
{
    return colors_.size();
}

std::string model::get_color_name(size_t index) const
{
    return colors_[index].name;
}

void model::set_color_name(size_t index, std::string const& name)
{
    colors_[index].name = name;
    color_changed_();
}

color model::get_color(size_t index) const
{
    return colors_[index].color;
}

void model::set_color(size_t index, color const& color)
{
    colors_[index].color = color;
    color_changed_();
}

sigc::signal<void,shape*>& model::signal_shape_deleted()
{
    return shape_deleted_;
}

sigc::signal<void>& model::signal_color_changed()
{
    return color_changed_;
}

} // namespace linjal
