#ifndef LINJAL_MODEL_HPP
#define LINJAL_MODEL_HPP

#include <sigc++/signal.h>
#include <vector>
#include "shape.hpp"

namespace linjal {

class model
{
public:
    shape& new_shape();
    void delete_shape(shape& shape);
    void delete_degenerate_shapes();
    shape* pick(cml::vector2f const& position);
    void draw(Cairo::RefPtr<Cairo::Context> const& cairo, camera const& camera);

    sigc::signal<void,shape*>& signal_shape_deleted();

private:
    std::vector<shape> shapes_;
    sigc::signal<void,shape*> shape_deleted_;
};

} // namespace linjal

#endif // LINJAL_MODEL_HPP
