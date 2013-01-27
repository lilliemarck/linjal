#include "color_palette_widget.hpp"
#include "utils.hpp"

namespace linjal {

color_palette_widget::color_palette_widget(model& model) :
    model_(model),
    color_palette_model_(model),
    tree_view_(this),
    add_button_(tr("Add")),
    remove_button_(tr("Remove"))
{
    setWindowTitle(tr("Color Palette"));
    resize(200, 300);

    tree_view_.setModel(&color_palette_model_);
    tree_view_.resizeColumnToContents(0);
    tree_view_.resizeColumnToContents(2);

    connect(&add_button_, SIGNAL(clicked()), this, SLOT(on_add_color()));
    connect(&remove_button_, SIGNAL(clicked()), this, SLOT(on_remove_color()));
    connect(&tree_view_, SIGNAL(activated(const QModelIndex&)), this, SLOT(on_item_activated(const QModelIndex&)));
    connect(tree_view_.selectionModel(), SIGNAL(currentChanged(QModelIndex const&, QModelIndex const&)), this, SLOT(on_current_changed(QModelIndex const&, QModelIndex const&)));
    connect(&color_dialog_, SIGNAL(currentColorChanged(QColor const&)), this, SLOT(on_color_changed(QColor const&)));
    connect(&color_dialog_, SIGNAL(rejected()), this, SLOT(on_color_rejected()));

    hbox_layout_.addWidget(&add_button_);
    hbox_layout_.addWidget(&remove_button_);
    vbox_layout_.addWidget(&tree_view_);
    vbox_layout_.addLayout(&hbox_layout_);
    setLayout(&vbox_layout_);

    color_dialog_.setModal(true);
    color_dialog_.setOptions(QColorDialog::ShowAlphaChannel);
}

void color_palette_widget::set_color_index(std::size_t index)
{
    tree_view_.setCurrentIndex(color_palette_model_.index(index));
}

void color_palette_widget::on_add_color()
{
    model_.new_color();
}

void color_palette_widget::on_remove_color()
{
    QItemSelectionModel const* selection = tree_view_.selectionModel();

    if (selection->hasSelection())
    {
        std::size_t row = selection->currentIndex().row();

        if (model_.color_count() > 1 && row < model_.color_count())
        {
            model_.delete_color(row);
        }
    }
}

void color_palette_widget::on_item_activated(QModelIndex const& index)
{
    if (std::size_t(index.row()) < model_.color_count())
    {
        color_index_ = index.row();
        original_color_ = model_.get_color(color_index_);
        color_dialog_.setCurrentColor(to_qcolor(original_color_));
        color_dialog_.show();
    }
}

void color_palette_widget::on_current_changed(QModelIndex const& current, QModelIndex const& previous)
{
    color_index_changed(current.row());
}

void color_palette_widget::on_color_changed(QColor const& color)
{
    if (color_index_ < model_.color_count())
    {
        model_.set_color(color_index_, to_color(color));
    }
}

void color_palette_widget::on_color_rejected()
{
    if (color_index_ < model_.color_count())
    {
        model_.set_color(color_index_, original_color_);
    }
}

} // namespace linjal
