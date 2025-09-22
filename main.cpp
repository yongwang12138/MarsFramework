#include <QApplication>
#include "MarsWindow.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MarsWindow window;
    window.show();

    Widget x = Widget();
    window.setMainWidget(&x);
    return a.exec();
}
