#pragma once

#include <QColorDialog>
#include <QPushButton>
#include <QTreeView>
#include <QVBoxLayout>
#include <QWidget>
#include "color_palette_model.hpp"

namespace linjal {

class model;

class color_palette_widget : public QWidget
{
    Q_OBJECT

public:
    color_palette_widget(model& model);

    void set_color_index(std::size_t index);

Q_SIGNALS:
    void color_index_changed(std::size_t index);

private Q_SLOTS:
    void on_add_color();
    void on_remove_color();
    void on_item_activated(QModelIndex const& index);
    void on_current_changed(QModelIndex const& current, QModelIndex const& previous);
    void on_color_changed(QColor const& color);
    void on_color_rejected();

private:
    model& model_;
    color_palette_model color_palette_model_;
    QTreeView tree_view_;
    QPushButton add_button_;
    QPushButton remove_button_;
    QVBoxLayout vbox_layout_;
    QHBoxLayout hbox_layout_;
    QColorDialog color_dialog_;
    std::size_t color_index_;
    color original_color_;
};

} // namespace linjal
