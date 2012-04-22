#include "main_window.hpp"
#include <gtkmm/aboutdialog.h>
#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/toolbar.h>

namespace linjal {

namespace
{
    static const Glib::ustring ui_definition =
        "<ui>"
        "  <menubar name='menu_bar'>"
        "    <menu action='file_menu'>"
        "      <menuitem action='quit'/>"
        "    </menu>"
        "    <menu action='help_menu'>"
        "      <menuitem action='about'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";
}

main_window::main_window() :
    ui_manager_(Gtk::UIManager::create()),
    action_group_(Gtk::ActionGroup::create())
{
    set_title("Linjal");
    set_default_size(640, 400);
    set_position(Gtk::WIN_POS_CENTER);

    add(vbox_);
    create_actions();
    show_all_children();
}

void main_window::create_actions()
{
    action_group_->add(Gtk::Action::create("file_menu", "_File"));
    action_group_->add(Gtk::Action::create("quit", Gtk::Stock::QUIT),
        sigc::ptr_fun(Gtk::Main::quit));

    action_group_->add(Gtk::Action::create("help_menu", "_Help"));
    action_group_->add(Gtk::Action::create("about", Gtk::Stock::ABOUT),
          sigc::mem_fun(this, &main_window::show_about_dialog));

    ui_manager_->insert_action_group(action_group_);
    ui_manager_->add_ui_from_string(ui_definition);

    if (Gtk::Widget* menu_bar = ui_manager_->get_widget("/menu_bar"))
    {
        vbox_.pack_start(*menu_bar, Gtk::PACK_SHRINK);
    }

    add_accel_group(ui_manager_->get_accel_group());
}

void main_window::show_about_dialog()
{
    std::vector<Glib::ustring> authors{"Jonathan Lilliemarck"};

    Gtk::AboutDialog dialog;
    dialog.set_program_name("Linjal");
    dialog.set_copyright("© 2012 Jonathan Lilliemarck");
    dialog.set_website("https://github.com/lilliemarck/linjal");
    dialog.set_website_label("Hosted on GitHub");
    dialog.set_authors(authors);
    dialog.run();
}

} // namespace linjal
