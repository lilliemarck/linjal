#pragma once

#include <QMainWindow>
#include "model.hpp"

namespace linjal {

class color_palette_window;

class main_window : public QMainWindow
{
public:
    main_window();
    ~main_window();

private:
    void create_actions();
    void show_palette();
    void show_color_chooser();
    void on_color_index_changed(size_t color_index);
    void show_about_dialog();
    void open();
    void save_as();
    void show_select_image_dialog();
    void show_export_dialog();

    model model_;
#if 0
    Glib::RefPtr<Gtk::UIManager> ui_manager_;
    Glib::RefPtr<Gtk::ToggleAction> show_image_action_;
    Glib::RefPtr<Gtk::ActionGroup> action_group_;
    Glib::RefPtr<Gtk::RadioAction> pen_tool_action_;
    Glib::RefPtr<Gtk::RadioAction> select_tool_action_;
    Gtk::VBox vbox_;
    drawing_area drawing_area_;

    std::unique_ptr<color_palette_window> color_palette_window_;
    std::unique_ptr<color_palette_window> color_palette_dialog_;
#endif
};

} // namespace linjal
