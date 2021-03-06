#include "main_window.hpp"
#include <json_spirit_reader.h>
#include <json_spirit_writer.h>
#include <fstream>
#include <QAction>
#include <QFileDialog>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include "color_palette_widget.hpp"

namespace linjal {

namespace
{
    void create_default_colors(model &model)
    {
        std::size_t colod_index = model.new_color();
        model.set_color_name(colod_index, "red");
        model.set_color(colod_index, {255, 0, 0, 255});
        colod_index = model.new_color();
        model.set_color_name(colod_index, "green");
        model.set_color(colod_index, {0, 255, 0, 255});
    }
}

main_window::main_window() : drawing_area_(model_)
{
    create_default_colors(model_);

    setWindowTitle("Linjal");
    resize(640, 400);

    setCentralWidget(&drawing_area_);

    create_actions();
    create_menus();
    create_toolbar();
}

main_window::~main_window()
{
}

void main_window::create_actions()
{
    open_action_ = new QAction(tr("&Open..."), this);
    open_action_->setShortcuts(QKeySequence::Open);
    connect(open_action_, SIGNAL(triggered()), this, SLOT(open()));

    save_as_action_ = new QAction(tr("Save &As..."), this);
    save_as_action_->setShortcuts(QKeySequence::SaveAs);
    connect(save_as_action_, SIGNAL(triggered()), this, SLOT(save_as()));

    open_image_action_ = new QAction(tr("Open Image..."), this);
    connect(open_image_action_, SIGNAL(triggered()), this, SLOT(show_select_image_dialog()));

    export_to_png_action_ = new QAction(tr("Export to PNG..."), this);
    connect(export_to_png_action_, SIGNAL(triggered()), this, SLOT(show_export_dialog()));

    quit_action_ = new QAction(tr("Quit"), this);
    connect(quit_action_, SIGNAL(triggered()), this, SLOT(close()));

    delete_action_ = new QAction(tr("&Delete"), this);
    delete_action_->setShortcut(QKeySequence::Delete);
    connect(delete_action_, SIGNAL(triggered()), &drawing_area_, SLOT(delete_selection()));

    show_image_action_ = new QAction(tr("Show Image"), this);
    show_image_action_->setCheckable(true);
    connect(show_image_action_, SIGNAL(toggled(bool)), &drawing_area_, SLOT(set_image_visible(bool)));

    move_shape_up_action_ = new QAction(tr("Move Shape Up"), this);
    move_shape_up_action_->setShortcut(QKeySequence(tr("PgUp")));
    connect(move_shape_up_action_, SIGNAL(triggered()), &drawing_area_, SLOT(move_shape_up()));

    move_shape_down_action_ = new QAction(tr("Move Shape Down"), this);
    move_shape_down_action_->setShortcut(QKeySequence(tr("PgDown")));
    connect(move_shape_down_action_, SIGNAL(triggered()), &drawing_area_, SLOT(move_shape_down()));

    new_shape_action_ = new QAction(tr("New Shape"), this);
    connect(new_shape_action_, SIGNAL(triggered()), &drawing_area_, SLOT(new_shape()));

    pen_action_ = new QAction(tr("Pen"), this);
    connect(pen_action_, SIGNAL(triggered()), &drawing_area_, SLOT(use_pen_tool()));

    select_action_ = new QAction(tr("Select"), this);
    connect(select_action_, SIGNAL(triggered()), &drawing_area_, SLOT(use_select_tool()));

    QActionGroup* tool_group = new QActionGroup(this);
    tool_group->addAction(pen_action_);
    tool_group->addAction(select_action_);

    palette_action_ = new QAction(tr("Palette"), this);
    connect(palette_action_, SIGNAL(triggered()), this, SLOT(show_palette()));

    color_action_ = new QAction(tr("Color"), this);
    connect(color_action_, SIGNAL(triggered()), this, SLOT(show_color_chooser()));
}

void main_window::create_menus()
{
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(open_action_);
    file_menu_->addAction(save_as_action_);
    file_menu_->addAction(open_image_action_);
    file_menu_->addAction(export_to_png_action_);
    file_menu_->addSeparator();
    file_menu_->addAction(quit_action_);

    edit_menu_ = menuBar()->addMenu(tr("&Edit"));
    edit_menu_->addAction(delete_action_);
    edit_menu_->addAction(show_image_action_);
    edit_menu_->addAction(move_shape_up_action_);
    edit_menu_->addAction(move_shape_down_action_);
}

void main_window::create_toolbar()
{
    toolbar_ = addToolBar(tr("Tools"));
    toolbar_->addAction(new_shape_action_);
    toolbar_->addAction(pen_action_);
    toolbar_->addAction(select_action_);
    toolbar_->addSeparator();
    toolbar_->addAction(palette_action_);
    toolbar_->addAction(color_action_);
}

void main_window::show_palette()
{
    if (!color_palette_window_)
    {
        color_palette_window_.reset(new color_palette_widget(model_));
    }

    color_palette_window_->show();
}

void main_window::show_color_chooser()
{
    shape *shape = drawing_area_.selected_shape();

    if (!shape)
    {
        return;
    }

    if (!color_palette_dialog_)
    {
        color_palette_dialog_.reset(new color_palette_widget(model_));
        connect(color_palette_dialog_.get(), SIGNAL(color_index_changed(std::size_t)), this, SLOT(on_color_index_changed(std::size_t)));
    }

    color_palette_dialog_->set_color_index(shape->color_index);
    color_palette_dialog_->show();
}

void main_window::on_color_index_changed(size_t color_index)
{
    shape *shape = drawing_area_.selected_shape();

    if (shape && color_index < model_.color_count())
    {
        shape->color_index = color_index;
        drawing_area_.update();
    }
}

void main_window::open()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Linjal Document..."), QString(), tr("Document (*.linjal)"));

    if (!filename.isEmpty())
    {
        try
        {
            std::ifstream file(filename.toStdString());
            json_spirit::Value value;
            read(file, value);
            model_ = from_json<model>(value);
        }
        catch (...)
        {
        }
    }
}

void main_window::save_as()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Save Document As..."), QString(), tr("Document (*.linjal)"));

    if (!filename.isEmpty())
    {
        json_spirit::Value value = to_json(model_);
        std::ofstream file(filename.toStdString());
        write(value, file, json_spirit::remove_trailing_zeros);
    }
}

void main_window::show_select_image_dialog()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open Image..."), QString(), tr("PNG Image (*.png)"));

    if (!filename.isEmpty())
    {
        QImage image(filename);

        if (!image.isNull())
        {
            drawing_area_.set_image(image);
        }
    }
}

void main_window::show_export_dialog()
{
    QString filename = QFileDialog::getSaveFileName(this, tr("Export Image..."), QString(), tr("PNG Image (*.png)"));

    if (!filename.isEmpty())
    {
        drawing_area_.draw_to_image().save(filename, "PNG");
    }
}

} // namespace linjal
