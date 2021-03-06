#pragma once

#include <linjal/json.hpp>
#include <sigc++/signal.h>
#include <vector>
#include "drawing_context.hpp"
#include "shape.hpp"

namespace linjal {

class model
{
public:
    model();
    shape& new_shape();
    void delete_shape(shape& shape);
    size_t shape_count() const;
    shape const& get_shape(std::size_t index) const;
    size_t index_of_shape(shape& shape);
    shape& replace_shape(shape& shape, size_t new_index);
    void delete_degenerate_shapes();
    shape* pick(math::vector2f const& position);
    void draw(drawing_context& context, camera const& camera);

    size_t new_color();
    void delete_color(size_t index);
    size_t color_count() const;
    std::size_t index_of_color(std::string const& name) const;
    std::string get_color_name(size_t index) const;
    void set_color_name(size_t index, std::string const& name);
    color get_color(size_t index) const;
    void set_color(size_t index, color const& color);

    sigc::signal<void,shape*>& signal_shape_deleted();
    sigc::signal<void,size_t>& signal_color_inserted();
    sigc::signal<void,size_t>& signal_color_deleted();
    sigc::signal<void,std::size_t>& signal_color_changed();

private:
    struct named_color
    {
        std::string name;
        linjal::color color;
    };

    std::vector<shape> shapes_;
    std::vector<named_color> colors_;
    sigc::signal<void,shape*> shape_deleted_;
    sigc::signal<void,size_t> color_inserted_;
    sigc::signal<void,size_t> color_deleted_;
    sigc::signal<void,std::size_t> color_changed_;
};

template <>
struct json_converter<model>
{
    static json_spirit::Value to_json(model const& model);
    static model from_json(json_spirit::Value const& value);
};

} // namespace linjal
