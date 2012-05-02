#include "color_palette_widget.hpp"
#include "color_palette_model.hpp"
#include "utils.hpp"

namespace linjal {

color_palette_widget::color_palette_widget(model& model) :
    model_(model),
    color_palette_model_(color_palette_model::create(model)),
    add_color_tool_button_("Add"),
    remove_color_tool_button("Remove")
{
    scrolled_window_.set_policy(Gtk::POLICY_NEVER, Gtk::POLICY_ALWAYS);

    pack_start(scrolled_window_);
    setup_tree_view();
    setup_toolbar();

    tree_view_.get_selection()->signal_changed().connect(sigc::mem_fun(this, &color_palette_widget::on_selection_changed));

    show_all_children();
}

color_palette_widget::~color_palette_widget()
{
}

void color_palette_widget::setup_tree_view()
{
    tree_view_.set_model(color_palette_model_);

    {
        name_cell_renderer_.property_editable() = true;
        name_cell_renderer_.signal_edited().connect(sigc::mem_fun(this, &color_palette_widget::on_name_edited));

        Gtk::TreeViewColumn* name_column = Gtk::manage(new Gtk::TreeViewColumn("Name", name_cell_renderer_));
        name_column->set_renderer(name_cell_renderer_, color_palette_model::columns().name);
        tree_view_.append_column(*name_column);
    }

    {
        color_cell_renderer_.property_mode() = Gtk::CELL_RENDERER_MODE_EDITABLE;
        color_cell_renderer_.signal_edited().connect(sigc::mem_fun(this, &color_palette_widget::on_color_edited));

        Gtk::TreeViewColumn* color_column = Gtk::manage(new Gtk::TreeViewColumn("Color", color_cell_renderer_));
        color_column->add_attribute(color_cell_renderer_.property_color(), color_palette_model::columns().color);
        tree_view_.append_column(*color_column);
    }

    scrolled_window_.add(tree_view_);
}

void color_palette_widget::setup_toolbar()
{
    add_color_tool_button_.set_icon_name("gtk-add");
    remove_color_tool_button.set_icon_name("gtk-remove");

    add_color_tool_button_.signal_clicked().connect(sigc::mem_fun(this, &color_palette_widget::on_add_color));
    remove_color_tool_button.signal_clicked().connect(sigc::mem_fun(this, &color_palette_widget::on_remove_color));

    toolbar_.insert(add_color_tool_button_, -1);
    toolbar_.insert(remove_color_tool_button, -1);
    toolbar_.set_icon_size(Gtk::ICON_SIZE_SMALL_TOOLBAR);

    pack_start(toolbar_, Gtk::PACK_SHRINK);
}

size_t color_palette_widget::get_selected_color_index()
{
    Gtk::TreeModel::iterator iter = tree_view_.get_selection()->get_selected();
    return color_palette_model_->get_index(iter);
}

void color_palette_widget::on_selection_changed()
{
    size_t color_index = get_selected_color_index();
    if (color_index < model_.color_count())
    {
        color_index_changed_(color_index);
    }
}

void color_palette_widget::on_name_edited(Glib::ustring const& path, Glib::ustring const& new_text)
{
    Gtk::TreeModel::Path tree_path(path);
    Gtk::TreeModel::iterator iter = color_palette_model_->get_iter(tree_path);

    if (iter)
    {
        size_t color_index = color_palette_model_->get_index(iter);
        if (color_index < model_.color_count())
        {
            model_.set_color_name(color_index, new_text);
//             color_palette_model_->row_changed(tree_path, iter);
        }
    }
}

void color_palette_widget::on_color_edited(Glib::ustring const& path, color const& color)
{
    size_t color_index_ = color_palette_model_->get_index(Gtk::TreeModel::Path(path));

    if (color_index_ < model_.color_count())
    {
        model_.set_color(color_index_, color);
    }
}

void color_palette_widget::on_add_color()
{
    size_t color_index = model_.new_color();
    Gtk::TreeModel::Path path(1, color_index);
    Gtk::TreeModel::iterator iter(color_palette_model_->get_iter(path));

    if (iter)
    {
        color_palette_model_->row_inserted(path, iter);
    }
}

void color_palette_widget::on_remove_color()
{
    size_t color_index = get_selected_color_index();

    if (model_.color_count() > 1 && color_index < model_.color_count())
    {
        model_.delete_color(color_index);
        color_palette_model_->row_deleted(Gtk::TreeModel::Path(1, color_index));
    }
}

} // namespace linjal
