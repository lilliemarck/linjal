#include "color_palette_model.hpp"
#include "utils.hpp"

namespace linjal {

color_palette_model::color_palette_model(model& model) : model_(model)
{
    model_.signal_color_inserted().connect(sigc::mem_fun(this, &color_palette_model::on_color_inserted));
    model_.signal_color_deleted().connect(sigc::mem_fun(this, &color_palette_model::on_color_deleted));
    model_.signal_color_changed().connect(sigc::mem_fun(this, &color_palette_model::on_color_changed));
}

QVariant color_palette_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        switch (section)
        {
        case name_column:
            return tr("Name");

        case color_column:
            return tr("Color");
        }
    }

    return QVariant();
}

int color_palette_model::rowCount(QModelIndex const &parent) const
{
    return model_.color_count();
}

int color_palette_model::columnCount(QModelIndex const &parent) const
{
    return 3;
}

Qt::ItemFlags color_palette_model::flags(QModelIndex const& index) const
{
    if (index.column() == name_column)
    {
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    }

    return QAbstractListModel::flags(index);
}

QVariant color_palette_model::data(QModelIndex const &index, int role) const
{
    switch (index.column())
    {
    case index_column:
        if (role == Qt::DisplayRole)
        {
            return index.row();
        }
        break;

    case name_column:
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return QString::fromUtf8(model_.get_color_name(index.row()).c_str());
        }
        break;

    case color_column:
        if (role == Qt::DecorationRole)
        {
            return to_qcolor(model_.get_color(index.row()));
        }
        break;
    }

    return QVariant();
}


bool color_palette_model::setData(QModelIndex const& index, QVariant const& value, int role)
{
    if (role == Qt::EditRole)
    {
        if (index.column() == name_column)
        {
            model_.set_color_name(index.row(), to_string(value));
            return true;
        }
    }

    return false;
}

void color_palette_model::on_color_inserted(std::size_t row)
{
    beginInsertRows(QModelIndex(), row, row);
    endInsertRows();
}

void color_palette_model::on_color_deleted(std::size_t row)
{
    beginRemoveRows(QModelIndex(), row, row);
    endRemoveRows();
}

void color_palette_model::on_color_changed(std::size_t row)
{
    dataChanged(index(row, name_column), index(row, color_column));
}

} // namespace linjal
