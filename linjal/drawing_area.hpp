#ifndef LINJAL_DRAWING_AREA_HPP
#define LINJAL_DRAWING_AREA_HPP

#include <gtkmm/drawingarea.h>

namespace linjal {

class drawing_area : public Gtk::DrawingArea
{
private:
    bool on_draw(Cairo::RefPtr<Cairo::Context> const& cairo) override;
};

} // namespace linjal

#endif // LINJAL_DRAWING_AREA_HPP
