#include <QApplication>
#include "MarsWidget.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MarsWidget window;
    window.show();

    Widget x = Widget();
    window.setMainWidget(&x);
    return a.exec();
}
