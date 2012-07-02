#include "main_window.hpp"
#include <gtkmm/aboutdialog.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/main.h>
#include <gtkmm/radioaction.h>
#include <gtkmm/stock.h>
#include <gtkmm/toolbar.h>
#include "color_palette_window.hpp"

namespace linjal {

namespace
{
    static const Glib::ustring ui_definition =
        "<ui>"
        "  <menubar name='menu-bar'>"
        "    <menu action='file-menu'>"
        "      <menuitem action='open-image'/>"
        "      <menuitem action='quit'/>"
        "    </menu>"
        "    <menu action='edit-menu'>"
        "      <menuitem action='delete'/>"
        "      <menuitem action='show-image'/>"
        "      <menuitem action='move-shape-up'/>"
        "      <menuitem action='move-shape-down'/>"
        "    </menu>"
        "    <menu action='help_menu'>"
        "      <menuitem action='about'/>"
        "    </menu>"
        "  </menubar>"
        "  <toolbar name='toolbar'>"
        "    <toolitem action='new-shape'/>"
        "    <toolitem action='pen'/>"
        "    <toolitem action='select'/>"
        "    <toolitem action='palette'/>"
        "    <toolitem action='color'/>"
        "  </toolbar>"
        "</ui>";
}

main_window::main_window() :
    ui_manager_(Gtk::UIManager::create()),
    action_group_(Gtk::ActionGroup::create()),
    drawing_area_(model_)
{
    set_title("Linjal");
    set_default_size(640, 400);
    set_position(Gtk::WIN_POS_CENTER);

    add(vbox_);
    create_actions();
    vbox_.pack_start(drawing_area_);
    show_all_children();
}

main_window::~main_window()
{
}

void main_window::create_actions()
{
    action_group_->add(Gtk::Action::create("file-menu", "_File"));
    action_group_->add(Gtk::Action::create("open-image", Gtk::Stock::OPEN),
        sigc::mem_fun(this, &main_window::show_select_image_dialog));
    action_group_->add(Gtk::Action::create("quit", Gtk::Stock::QUIT),
        sigc::ptr_fun(Gtk::Main::quit));

    action_group_->add(Gtk::Action::create("edit-menu", "_Edit"));
    action_group_->add(Gtk::Action::create("delete", Gtk::Stock::DELETE),
        Gtk::AccelKey("Delete"),
        sigc::mem_fun(drawing_area_, &drawing_area::delete_selection));

    show_image_action_ = Gtk::ToggleAction::create("show-image", "Show Image", Glib::ustring(), true);
    action_group_->add(show_image_action_, [=]()
    {
        drawing_area_.set_image_visible(show_image_action_->get_active());
    });

    action_group_->add(Gtk::Action::create("move-shape-up", "Move Shape Up"),
                       Gtk::AccelKey("Page_Up"),
                       sigc::mem_fun(drawing_area_, &drawing_area::move_shape_up));

    action_group_->add(Gtk::Action::create("move-shape-down", "Move Shape Up"),
                       Gtk::AccelKey("Page_Down"),
                       sigc::mem_fun(drawing_area_, &drawing_area::move_shape_down));

    action_group_->add(Gtk::Action::create("help-menu", "_Help"));
    action_group_->add(Gtk::Action::create("about", Gtk::Stock::ABOUT),
          sigc::mem_fun(this, &main_window::show_about_dialog));

    action_group_->add(Gtk::Action::create("new-shape", "New Shape"),
                       sigc::mem_fun(drawing_area_, &drawing_area::new_shape));
    action_group_->add(Gtk::Action::create("palette", "Palette"),
                       sigc::mem_fun(this, &main_window::show_palette));
    action_group_->add(Gtk::Action::create("color", "Color"),
                       sigc::mem_fun(this, &main_window::show_color_chooser));

    Gtk::RadioAction::Group tool_group;
    pen_tool_action_ = Gtk::RadioAction::create(tool_group, "pen", "Pen");
    select_tool_action_ = Gtk::RadioAction::create(tool_group, "select", "Select");

    action_group_->add(pen_tool_action_, [=]()
    {
        if (pen_tool_action_->get_active())
        {
            drawing_area_.use_pen_tool();
        }
    });

    action_group_->add(select_tool_action_, [=]()
    {
        if (select_tool_action_->get_active())
        {
            drawing_area_.use_select_tool();
        }
    });

    ui_manager_->insert_action_group(action_group_);
    ui_manager_->add_ui_from_string(ui_definition);

    if (Gtk::Widget* menu_bar = ui_manager_->get_widget("/menu-bar"))
    {
        vbox_.pack_start(*menu_bar, Gtk::PACK_SHRINK);
    }

    if (Gtk::Toolbar* toolbar = dynamic_cast<Gtk::Toolbar*>(ui_manager_->get_widget("/toolbar")))
    {
        toolbar->set_toolbar_style(Gtk::TOOLBAR_TEXT);
        vbox_.pack_start(*toolbar, Gtk::PACK_SHRINK);
    }

    add_accel_group(ui_manager_->get_accel_group());
}

void main_window::show_palette()
{
    if (!color_palette_window_)
    {
        color_palette_window_ = std::unique_ptr<color_palette_window>(new color_palette_window(model_));
    }

    color_palette_window_->present();
}

void main_window::show_color_chooser()
{
    if (!color_palette_dialog_)
    {
        color_palette_dialog_ = std::unique_ptr<color_palette_window>(new color_palette_window(model_));
        color_palette_dialog_->set_title("Select Color");
        color_palette_dialog_->get_color_palette_widget().signal_color_index_changed().connect(sigc::mem_fun(this, &main_window::on_color_index_changed));
    }

    color_palette_dialog_->present();
}

void main_window::on_color_index_changed(size_t color_index)
{
    shape *shape = drawing_area_.selected_shape();

    if (shape && color_index < model_.color_count())
    {
        shape->color_index = color_index;
        drawing_area_.queue_draw();
    }
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

void main_window::show_select_image_dialog()
{
    Gtk::FileChooserDialog dialog("Open File...");
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_ACCEPT);

    if (dialog.run() == Gtk::RESPONSE_ACCEPT)
    {
        try
        {
            auto image = Cairo::ImageSurface::create_from_png(dialog.get_filename());
            drawing_area_.set_image(image);
        }
        catch (...)
        {
        }
    }
}

} // namespace linjal
