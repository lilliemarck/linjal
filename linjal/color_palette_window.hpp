#pragma once

#include <gtkmm/window.h>
#include "color_palette_widget.hpp"

namespace linjal {

class model;

/**
 * Displays a ColorPaletteWidget in a Window.
 */
class color_palette_window : public Gtk::Window
{
public:
    color_palette_window(model& model);
    color_palette_widget& get_color_palette_widget();

private:
    color_palette_widget color_palette_widget_;
};

} // namespace linjal
