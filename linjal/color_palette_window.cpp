#include "color_palette_window.hpp"

namespace linjal {

color_palette_window::color_palette_window(model& model) :
    color_palette_widget_(model)
{
    set_title("Color Palette");
    set_default_size(200, 300);
    set_position(Gtk::WIN_POS_CENTER);
    add(color_palette_widget_);
    show_all_children();
}

} // namespace linjal
