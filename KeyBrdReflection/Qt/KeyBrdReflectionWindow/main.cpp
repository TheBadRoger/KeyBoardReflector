#include "KeyBrdReflectionWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KeyBrdReflectionWindow window;

    window.show();
    window.ShowTimeOnBox();
    return app.exec();
}
