#include <gtkmm/main.h>
#include <gtkmm/window.h>

int main(int argc, char** argv)
{
    Gtk::Main main(argc, argv);
    Gtk::Window window;
    window.set_title("Linjal");
    window.set_default_size(640, 400);
    window.set_position(Gtk::WIN_POS_CENTER);
    main.run(window);
    return 0;
}
