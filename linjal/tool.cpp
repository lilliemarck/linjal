#include "tool.hpp"
#include "drawing_area.hpp"

namespace linjal {

tool::tool(drawing_area* drawing_area) :
    drawing_area_(drawing_area),
    model_(drawing_area->model_)
{
}

} // namespace linjal
