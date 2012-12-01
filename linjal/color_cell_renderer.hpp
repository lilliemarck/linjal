#ifndef LINJAL_COLOR_CELL_RENDERER_HPP
#define LINJAL_COLOR_CELL_RENDERER_HPP

#include <glibmm/property.h>
#include <gtkmm/cellrenderer.h>

namespace linjal {

struct color;

class color_cell_renderer : public Gtk::CellRenderer
{
public:
    color_cell_renderer();
    Glib::PropertyProxy<color> property_color();
    sigc::signal<void,Glib::ustring const&,color const&> signal_edited();

private:
    void render_vfunc(Cairo::RefPtr<Cairo::Context> const& cairo, Gtk::Widget& widget, Gdk::Rectangle const& background_area, Gdk::Rectangle const& cell_area, Gtk::CellRendererState flags) override;
    Gtk::CellEditable* start_editing_vfunc(GdkEvent* event, Gtk::Widget& widget, const Glib::ustring& path, const Gdk::Rectangle& background_area, const Gdk::Rectangle& cell_area, Gtk::CellRendererState flags) override;

    Glib::Property<color> color_;
    sigc::signal<void,Glib::ustring const&,color const&> edited_;
};

} // namespace linjal

#endif // LINJAL_COLOR_CELL_RENDERER_HPP
