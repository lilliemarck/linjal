#pragma once

#include <QAbstractListModel>
#include "model.hpp"

namespace linjal {

class color_palette_model : public QAbstractListModel
{
public:
    enum columns
    {
        index_column,
        name_column,
        color_column,
        columns_
    };

    color_palette_model(model& model);

    int columnCount(QModelIndex const &parent) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    int rowCount(QModelIndex const &parent) const override;
    Qt::ItemFlags flags(QModelIndex const& index) const override;
    QVariant data(QModelIndex const &index, int role) const override;
    bool setData(QModelIndex const& index, QVariant const& value, int role) override;

private:
    void on_color_inserted(std::size_t row);
    void on_color_deleted(std::size_t row);
    void on_color_changed(std::size_t row);

    model& model_;
};

} // namespace linjal
