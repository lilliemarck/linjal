#include "model.hpp"
#include <QImage>
#include <QPainter>
#include "camera.hpp"
#include "shape.hpp"

namespace linjal {

model::model()
{
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
    QImage image(0, 0, QImage::Format_RGB32);
    QPainter painter(&image);
    drawing_context context(painter);
    camera no_transform;

    for (auto iter = begin(shapes_); iter != end(shapes_);)
    {
        auto& shape = *iter;
        path_curve(shape.path, context, no_transform);

        rect extents = context.get_fill_extents();
        context.fill();

        if (extents.size.x() == 0.0f && extents.size.y() == 0.0f)
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

shape* model::pick(math::vector2f const& position)
{
    QImage image(0, 0, QImage::Format_RGB32);
    QPainter painter(&image);
    drawing_context context(painter);
    camera no_transform;
    shape* picked_shape = nullptr;

    for (auto& shape : shapes_)
    {
        path_curve(shape.path, context, no_transform);

        if (context.in_fill(position))
        {
            picked_shape = &shape;
        }

        context.fill();
    };

    return picked_shape;
}

void model::draw(drawing_context& context, camera const& camera)
{
    for (auto const& shape : shapes_)
    {
        context.set_color(colors_[shape.color_index].color);
        path_curve(shape.path, context, camera);
        context.fill();
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

std::size_t model::index_of_color(std::string const& name) const
{
    for (std::size_t i = 0; i < colors_.size(); ++i)
    {
        if (colors_[i].name == name)
        {
            return i;
        }
    }

    return std::numeric_limits<std::size_t>::max();
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
            path_array.push_back(node.position.x());
            path_array.push_back(node.position.y());
            path_array.push_back(node.control_point.x());
            path_array.push_back(node.control_point.y());
        }

        shape_object.emplace_back("path", std::move(path_array));
        shapes_array.push_back(std::move(shape_object));
    }

    model_object.emplace_back("shapes", std::move(shapes_array));
    return model_object;
}

model json_converter<model>::from_json(json_spirit::Value const& value)
{
    model model;
    json_spirit::Object const& model_object = value.get_obj();

    if (json_spirit::Array const* colors_array = get_array_ptr(model_object, "colors"))
    {
        for (auto const& color_value : *colors_array)
        {
            json_spirit::Object const& color_object = color_value.get_obj();

            std::string name = get_string(color_object, "name");
            color color =
            {
                get_uint8_t(color_object, "red"),
                get_uint8_t(color_object, "green"),
                get_uint8_t(color_object, "blue"),
                get_uint8_t(color_object, "alpha")
            };

            size_t color_index = model.new_color();
            model.set_color_name(color_index, name);
            model.set_color(color_index, color);
        }
    }

    if (json_spirit::Array const *shapes_array = get_array_ptr(model_object, "shapes"))
    {
        for (auto const& shape_value : *shapes_array)
        {
            json_spirit::Object const& shape_object = shape_value.get_obj();
            shape& shape = model.new_shape();
            shape.color_index = model.index_of_color(get_string(shape_object, "color"));
            json_spirit::Array const& path_array = get_array(shape_object, "path");

            for (std::size_t i = 0; i < path_array.size();)
            {
                node node;

                node.position.x() = path_array.at(i++).get_real();
                node.position.y() = path_array.at(i++).get_real();
                node.control_point.x() = path_array.at(i++).get_real();
                node.control_point.y() = path_array.at(i++).get_real();

                shape.path.push_back(node);
            }
        }
    }

    return model;
}

} // namespace linjal
