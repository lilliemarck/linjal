#include "color_palette_model.hpp"
#include <cassert>
#include <limits>
#include "utils.hpp"

namespace linjal {

namespace
{
    size_t iter_to_index(Gtk::TreeModel::iterator const& iter)
    {
        return GPOINTER_TO_INT(iter.gobj()->user_data);
    }

    void index_to_iter(Gtk::TreeModel::iterator& iter, size_t index)
    {
        GtkTreeIter* giter = iter.gobj();
        giter->stamp = 1;
        giter->user_data = GINT_TO_POINTER(index);
    }

    template <typename T>
    void set_glib_value(Glib::ValueBase &lhs, const T &rhs)
    {
        Glib::Value<T> temp;
        temp.init(Glib::Value<T>::value_type());
        temp.set(rhs);

        lhs.init(Glib::Value<T>::value_type());
        lhs = temp;
    }
} // namespace

color_palette_model::color_palette_model(model& model) :
    Glib::ObjectBase(typeid(color_palette_model)),
    model_(model)
{}

Glib::RefPtr<color_palette_model> color_palette_model::create(model& model)
{
    return Glib::RefPtr<color_palette_model>(new color_palette_model(model));
}

color_palette_columns const& color_palette_model::columns()
{
    static color_palette_columns s_columns;
    return s_columns;
}

size_t color_palette_model::get_index(Gtk::TreeModel::iterator const& iter)
{
    return iter ? iter_to_index(iter) : std::numeric_limits<size_t>::max();
}

size_t color_palette_model::get_index(Path const& path)
{
    return get_index(get_iter(path));
}

Gtk::TreeModelFlags color_palette_model::get_flags_vfunc() const
{
    return Gtk::TREE_MODEL_LIST_ONLY;
}

int color_palette_model::get_n_columns_vfunc() const
{
    return columns().size();
}

GType color_palette_model::get_column_type_vfunc(int index) const
{
    return columns().types()[index];
}

bool color_palette_model::iter_next_vfunc(iterator const& iter, iterator& iter_next) const
{
    size_t index = iter_to_index(iter) + 1;

    if (index < model_.color_count())
    {
        index_to_iter(iter_next, index);
        return true;
    }

    return false;
}

bool color_palette_model::get_iter_vfunc(Path const& path, iterator& iter) const
{
    if (path.size() == 1)
    {
        size_t index = path[0];

        if (index < model_.color_count())
        {
            index_to_iter(iter, index);
            return true;
        }
    }

    return false;
}

bool color_palette_model::iter_children_vfunc(iterator const& parent, iterator& iter) const
{
    return false;
}

bool color_palette_model::iter_parent_vfunc(iterator const& child, iterator& iter) const
{
    return false;
}

bool color_palette_model::iter_nth_child_vfunc(iterator const& parent, int n, iterator& iter) const
{
    return false;
}

bool color_palette_model::iter_nth_root_child_vfunc(int n, iterator& iter) const
{
    if (size_t(n) < model_.color_count())
    {
        index_to_iter(iter, n);
        return true;
    }

    return false;
}

bool color_palette_model::iter_has_child_vfunc(iterator const& iter) const
{
    return false;
}

int color_palette_model::iter_n_children_vfunc(iterator const& iter) const
{
    return 0;
}

int color_palette_model::iter_n_root_children_vfunc() const
{
    return model_.color_count();
}

void color_palette_model::ref_node_vfunc(iterator const& iter) const
{
}

void color_palette_model::unref_node_vfunc(iterator const& iter) const
{
}

color_palette_model::Path color_palette_model::get_path_vfunc(iterator const& iter) const
{
    return Path(1, iter_to_index(iter));
}

void color_palette_model::get_value_vfunc(iterator const& iter, int column, Glib::ValueBase& value) const
{
    int index = iter_to_index(iter);

    switch (column)
    {
        case 0:
            set_glib_value<size_t>(value, index);
        case 1:
            set_glib_value<std::string>(value, model_.get_color_name(index));
            break;
        case 2:
            set_glib_value<color>(value, model_.get_color(index));
            break;
    }
}

} // namespace linjal
