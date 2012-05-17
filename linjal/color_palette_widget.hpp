#ifndef LINJAL_COLOR_PALETTE_WIDGET_HPP
#define LINJAL_COLOR_PALETTE_WIDGET_HPP

#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/toolbar.h>
#include <gtkmm/treeview.h>
#include "color_cell_renderer.hpp"

namespace linjal {

class color_palette_model;
class model;

class color_palette_widget : public Gtk::VBox
{
public:
    color_palette_widget(model& model);
    ~color_palette_widget();

    size_t get_selected_color_index();
    sigc::signal<void,size_t>& signal_color_index_changed();

private:
    void setup_tree_view();
    void setup_toolbar();

    void on_selection_changed();
    void on_name_edited(Glib::ustring const& path, Glib::ustring const& new_text);
    void on_color_edited(Glib::ustring const& path, color const& color);
    void on_add_color();
    void on_remove_color();

    model& model_;
    Glib::RefPtr<color_palette_model> color_palette_model_;
    Gtk::ScrolledWindow scrolled_window_;

    Gtk::TreeView tree_view_;
    Gtk::CellRendererText name_cell_renderer_;
    color_cell_renderer color_cell_renderer_;

    Gtk::Toolbar toolbar_;
    Gtk::ToolButton add_color_tool_button_;
    Gtk::ToolButton remove_color_tool_button;

    sigc::signal<void,size_t> color_index_changed_;
};

} // namespace linjal

#endif // LINJAL_COLOR_PALETTE_WIDGET_HPP
