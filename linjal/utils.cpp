#include "utils.hpp"
#include <QColor>
#include <QVariant>
#include "drawing_area.hpp"

namespace linjal {

std::string to_string(QVariant const& value)
{
    return value.toString().toUtf8().constData();
}

color to_color(QColor const& color)
{
    return {std::uint8_t(color.red()),
            std::uint8_t(color.green()),
            std::uint8_t(color.blue()),
            std::uint8_t(color.alpha())};
}

QColor to_qcolor(color const& color)
{
    return {color.r, color.g, color.b, color.a};
}

} // namespace linjal
