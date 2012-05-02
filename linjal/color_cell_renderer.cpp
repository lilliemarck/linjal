#include "color_cell_renderer.hpp"
#include <gtkmm/colorselection.h>
#include "model.hpp"

namespace linjal {

namespace
{
    color get_color(Gtk::ColorSelection const& selection)
    {
        Gdk::Color gdk_color = selection.get_current_color();
        guint16 alpha = selection.get_current_alpha();
        return {uint8_t(gdk_color.get_red() >> 8),
                uint8_t(gdk_color.get_green() >> 8),
                uint8_t(gdk_color.get_blue() >> 8),
                uint8_t(alpha >> 8)};
    }

    void set_color(Gtk::ColorSelection& selection, color const& color)
    {
        Gdk::Color gdk_color;
        gdk_color.set_rgb(guint16(257 * color.r),
                          guint16(257 * color.g),
                          guint16(257 * color.b));
        selection.set_current_color(gdk_color);
        selection.set_current_alpha(guint16(257 * color.a));
    }
} // namespace

color_cell_renderer::color_cell_renderer() :
    Glib::ObjectBase(typeid(color_cell_renderer)),
    color_(*this, "color")
{
}

Glib::PropertyProxy<color> color_cell_renderer::property_color()
{
    return color_.get_proxy();
}

sigc::signal<void,Glib::ustring const&,color const&> color_cell_renderer::signal_edited()
{
    return edited_;
}

void color_cell_renderer::render_vfunc(Cairo::RefPtr<Cairo::Context> const& cairo, Gtk::Widget& widget, Gdk::Rectangle const& background_area, Gdk::Rectangle const& cell_area, Gtk::CellRendererState flags)
{
    color color = color_.get_value();

    cairo->rectangle(cell_area.get_x() + 0.5, cell_area.get_y() + 0.5,
                     cell_area.get_width() - 1.0, cell_area.get_height() - 1.0);

    cairo->set_source_rgb(color.r / 255.0, color.g / 255.0, color.b / 255.0);
    cairo->fill_preserve();

    cairo->set_source_rgb(0.0, 0.0, 0.0);
    cairo->set_line_width(1);
    cairo->stroke();
}

Gtk::CellEditable* color_cell_renderer::start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags)
{
    color original_color = color_.get_value();

    Gtk::ColorSelectionDialog dialog;
    Gtk::ColorSelection& color_selection = *dialog.get_color_selection();
    color_selection.set_has_opacity_control(true);
    set_color(color_selection, original_color);

    color_selection.signal_color_changed().connect([&]()
    {
        color color = get_color(color_selection);
        edited_(path, color);
    });

    if (dialog.run() == Gtk::RESPONSE_CANCEL)
    {
        edited_(path, original_color);
    }

    return nullptr;
}

} // namespace linjal
