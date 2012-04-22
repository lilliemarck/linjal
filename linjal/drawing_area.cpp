#include "drawing_area.hpp"

namespace linjal {

bool drawing_area::on_draw(Cairo::RefPtr<Cairo::Context> const& cairo)
{
    cairo->set_source_rgb(1.0, 1.0, 1.0);
    cairo->paint();
    cairo->set_source_rgb(0.0, 0.0, 0.0);
    cairo->move_to(0.0, 0.0);
    cairo->line_to(get_width(), get_height());
    cairo->set_line_width(1.0);
    cairo->stroke();
    return true;
}

} // namespace linjal
