#pragma once

#include <QWidget>
#include <memory>
#include "camera.hpp"
#include "model.hpp"
#include "shape.hpp"
#include "tool.hpp"

namespace linjal {

class drawing_area : public QWidget
{
    Q_OBJECT

public:
    drawing_area(model &model);
    shape* selected_shape();
    void set_image(QImage const& image);
    QImage draw_to_image();

public Q_SLOTS:
    void new_shape();
    void use_pen_tool();
    void use_select_tool();
    void delete_selection();
    void set_image_visible(bool visible);
    void move_shape_up();
    void move_shape_down();

private:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void on_shape_deleted(shape* shape);
    void on_color_changed(std::size_t row);
    void draw_image(QPainter& painter) const;

    model& model_;
    shape* shape_;
    camera camera_;
    std::unique_ptr<tool> tool_;
    bool panning_;
    math::vector2f grab_position_;
    QImage image_;
    bool image_visible_;

    friend class tool;
    friend class pen_tool;
    friend class select_tool;
};

} // namespace linjal
