#pragma once

#include <QMainWindow>
#include "model.hpp"

namespace linjal {

class color_palette_window;

class main_window : public QMainWindow
{
    Q_OBJECT

public:
    main_window();
    ~main_window();

private Q_SLOTS:
    void show_palette();
    void show_color_chooser();
    void open();
    void save_as();
    void show_select_image_dialog();
    void show_export_dialog();

private:
    void create_actions();
    void create_menus();
    void create_toolbar();
    void on_color_index_changed(size_t color_index);

    model model_;

    QAction* open_action_;
    QAction* save_as_action_;
    QAction* open_image_action_;
    QAction* export_to_png_action_;
    QAction* quit_action_;
    QAction* delete_action_;
    QAction* show_image_action_;
    QAction* move_shape_up_action_;
    QAction* move_shape_down_action_;
    QAction* new_shape_action_;
    QAction* pen_action_;
    QAction* select_action_;
    QAction* palette_action_;
    QAction* color_action_;

    QMenu* file_menu_;
    QMenu* edit_menu_;
    QToolBar* toolbar_;

#if 0
    Gtk::VBox vbox_;
    drawing_area drawing_area_;

    std::unique_ptr<color_palette_window> color_palette_window_;
    std::unique_ptr<color_palette_window> color_palette_dialog_;
#endif
};

} // namespace linjal
