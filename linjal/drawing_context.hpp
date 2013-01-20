#pragma once

#include <QColor>
#include <QPainterPath>
#include <math/vector.hpp>

class QPainter;

namespace linjal {

struct color
{
    uint8_t r, g, b, a;
};

struct rect
{
    math::vector2f position;
    math::vector2f size;
};

class drawing_context
{
public:
    drawing_context(QPainter& painter);

    void move_to(math::vector2f const& point);
    void curve_to(math::vector2f const& p1, math::vector2f const& p2, math::vector2f const& p3);
    void circle(math::vector2f const& center, float radius);
    void set_color(color const& color);
    void fill();

    rect get_fill_extents();
    bool in_fill(math::vector2f const& point);

private:
    QPainter& painter_;
    QPainterPath path_;
    QColor color_;
};

} // namespace linjal
