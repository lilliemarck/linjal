#pragma once

#include <gtkmm.h>
#include "model.hpp"

namespace linjal {

struct color_palette_columns : public Gtk::TreeModel::ColumnRecord
{
    color_palette_columns() { add(index), add(name); add(color); }

    Gtk::TreeModelColumn<size_t> index;
    Gtk::TreeModelColumn<std::string> name;
    Gtk::TreeModelColumn<linjal::color> color;
};

class color_palette_model : public Glib::Object, public Gtk::TreeModel
{
public:
    static Glib::RefPtr<color_palette_model> create(model& model);
    static color_palette_columns const& columns();

    size_t get_index(Gtk::TreeModel::iterator const& iter);
    size_t get_index(Path const& path);

private:
    color_palette_model(model& model);

    Gtk::TreeModelFlags get_flags_vfunc() const override;
    int get_n_columns_vfunc() const override;
    GType get_column_type_vfunc(int index) const override;
    bool iter_next_vfunc(iterator const& iter, iterator& iter_next) const override;
    bool get_iter_vfunc(Path const& path, iterator& iter) const override;
    bool iter_children_vfunc(iterator const& parent, iterator& iter) const override;
    bool iter_parent_vfunc(iterator const& child, iterator& iter) const override;
    bool iter_nth_child_vfunc(iterator const& parent, int n, iterator& iter) const override;
    bool iter_nth_root_child_vfunc(int n, iterator& iter) const override;
    bool iter_has_child_vfunc(iterator const& iter) const override;
    int iter_n_children_vfunc(iterator const& iter) const override;
    int iter_n_root_children_vfunc() const override;
    void ref_node_vfunc(iterator const& iter) const override;
    void unref_node_vfunc(iterator const& iter) const override;
    Path get_path_vfunc(iterator const& iter) const override;
    void get_value_vfunc(iterator const& iter, int column, Glib::ValueBase& value) const override;

    void on_color_inserted(size_t index);
    void on_color_deleted(size_t index);

    model& model_;
};

} // namespace linjal
