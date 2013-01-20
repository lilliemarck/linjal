#include <QApplication>
#include "main_window.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    linjal::main_window window;
    window.show();
    return app.exec();
}
