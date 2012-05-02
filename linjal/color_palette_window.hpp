#ifndef LINJAL_COLOR_PALETTE_WINDOW_HPP
#define LINJAL_COLOR_PALETTE_WINDOW_HPP

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

private:
    color_palette_widget color_palette_widget_;
};

} // namespace linjal

#endif // LINJAL_COLOR_PALETTE_WINDOW_HPP
