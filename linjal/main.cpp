#include <gtkmm/main.h>
#include "main_window.hpp"

int main(int argc, char** argv)
{
    Gtk::Main main(argc, argv);
    linjal::main_window window;
    main.run(window);
    return 0;
}
