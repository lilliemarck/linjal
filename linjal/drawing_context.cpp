#include "drawing_context.hpp"
#include <QPainter>

namespace linjal {

drawing_context::drawing_context(QPainter &painter) : painter_(painter)
{
}

void drawing_context::move_to(math::vector2f const& point)
{
    path_.moveTo(point.x(), point.y());
}

void drawing_context::curve_to(math::vector2f const& p1, math::vector2f const& p2, math::vector2f const& p3)
{
    path_.cubicTo(p1.x(), p1.y(), p2.x(), p2.y(), p3.x(), p3.y());
}

void drawing_context::circle(math::vector2f const& center, float radius)
{
    float x = center.x() - radius;
    float y = center.y() - radius;
    float diameter = 2.0f * radius;

    path_.arcTo(x, y, diameter, diameter, 0.0, 360.0);
}

void drawing_context::set_color(color const& color)
{
    color_.setRgb(color.r, color.g, color.b, color.a);
}

void drawing_context::fill()
{
    painter_.fillPath(path_, color_);
    path_ = QPainterPath();
}

rect drawing_context::get_fill_extents()
{
    QRectF rect = path_.boundingRect();
    return {{float(rect.x()),     float(rect.y())},
            {float(rect.width()), float(rect.height())}};
}

bool drawing_context::in_fill(math::vector2f const& point)
{
    return path_.contains({point.x(), point.y()});
}

} // namespace linjal
