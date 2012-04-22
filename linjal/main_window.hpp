#ifndef LINJAL_MAIN_WINDOW_HPP
#define LINJAL_MAIN_WINDOW_HPP

#include <gtkmm/uimanager.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include "drawing_area.hpp"

namespace linjal {

class main_window : public Gtk::Window
{
public:
    main_window();

private:
    void create_actions();
    void show_about_dialog();

    Glib::RefPtr<Gtk::UIManager> ui_manager_;
    Glib::RefPtr<Gtk::ActionGroup> action_group_;
    Gtk::VBox vbox_;
    drawing_area drawing_area_;
};

} // namespace linjal

#endif // LINJAL_MAIN_WINDOW_HPP
