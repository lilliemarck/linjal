#pragma once

#include <string>

class QColor;
class QVariant;

namespace linjal {

struct color;

std::string to_string(QVariant const& value);

color to_color(QColor const& color);
QColor to_qcolor(color const& color);

} // namespace linjal
