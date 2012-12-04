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

size_t model::shape_count() const
{
    return shapes_.size();
}

shape const& model::get_shape(std::size_t index) const
{
    return shapes_[index];
}

size_t model::index_of_shape(shape& shape)
{
    return &shape - shapes_.data();
}

shape& model::replace_shape(shape& shape, size_t new_index)
{
    linjal::shape& new_shape = shapes_[new_index];
    std::swap(new_shape, shape);
    return new_shape;
}

void model::delete_degenerate_shapes()
{
    auto cairo = create_null_cairo_context();
    camera no_transform;

    for (auto iter = begin(shapes_); iter != end(shapes_);)
    {
        auto& shape = *iter;
        path_curve(shape.path, cairo, no_transform);

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
        path_curve(shape.path, cairo, no_transform);

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
    for (auto const& shape : shapes_)
    {
        color const& color = colors_[shape.color_index].color;
        cairo->set_source_rgb(color.r / 255.0, color.g / 255.0, color.b / 255.0);
        path_curve(shape.path, cairo, camera);
        cairo->fill();
    }
}

size_t model::new_color()
{
    size_t index  = colors_.size();
    colors_.push_back({"color", {0, 0, 255, 255}});
    color_inserted_(index);
    return index;
}

void model::delete_color(size_t index)
{
    colors_.erase(colors_.begin() + index);
    color_deleted_(index);
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

sigc::signal<void,size_t>& model::signal_color_inserted()
{
    return color_inserted_;
}

sigc::signal<void,size_t>& model::signal_color_deleted()
{
    return color_deleted_;
}

sigc::signal<void>& model::signal_color_changed()
{
    return color_changed_;
}

json_spirit::Value json_converter<model>::to_json(model const& model)
{
    json_spirit::Object model_object;
    json_spirit::Array colors_array;

    for (std::size_t i = 0; i < model.color_count(); ++i)
    {
        json_spirit::Object color_object;
        color_object.emplace_back("name", model.get_color_name(i));
        color const& color = model.get_color(i);
        color_object.emplace_back("red", color.r);
        color_object.emplace_back("green", color.g);
        color_object.emplace_back("blue", color.b);
        color_object.emplace_back("alpha", color.a);
        colors_array.push_back(std::move(color_object));
    }

    model_object.emplace_back("colors", std::move(colors_array));
    json_spirit::Array shapes_array;

    for (std::size_t i = 0; i < model.shape_count(); ++i)
    {
        json_spirit::Object shape_object;
        shape const& shape = model.get_shape(i);
        shape_object.emplace_back("color", model.get_color_name(shape.color_index));
        json_spirit::Array path_array;

        for (auto const& node : shape.path)
        {
            path_array.push_back(node.position[0]);
            path_array.push_back(node.position[1]);
            path_array.push_back(node.control_point[0]);
            path_array.push_back(node.control_point[1]);
        }

        shape_object.emplace_back("path", std::move(path_array));
        shapes_array.push_back(std::move(shape_object));
    }

    model_object.emplace_back("shapes", std::move(shapes_array));
    return model_object;
}

} // namespace linjal
