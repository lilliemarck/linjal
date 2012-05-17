#ifndef LINJAL_MODEL_HPP
#define LINJAL_MODEL_HPP

#include <sigc++/signal.h>
#include <vector>
#include "shape.hpp"

namespace linjal {

struct color
{
    uint8_t r, g, b, a;
};

class model
{
public:
    model();
    shape& new_shape();
    void delete_shape(shape& shape);
    void delete_degenerate_shapes();
    shape* pick(cml::vector2f const& position);
    void draw(Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera);

    size_t new_color();
    void delete_color(size_t index);
    size_t color_count() const;
    std::string get_color_name(size_t index) const;
    void set_color_name(size_t index, std::string const& name);
    color get_color(size_t index) const;
    void set_color(size_t index, color const& color);

    sigc::signal<void,shape*>& signal_shape_deleted();
    sigc::signal<void,size_t>& signal_color_inserted();
    sigc::signal<void,size_t>& signal_color_deleted();
    sigc::signal<void>& signal_color_changed();

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
    sigc::signal<void> color_changed_;
};

} // namespace linjal

#endif // LINJAL_MODEL_HPP
