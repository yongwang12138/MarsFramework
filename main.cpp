#include "MarsWidget.h"

#include <QVBoxLayout>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MarsWidget window;
    window.titleBar()->setTitle("MarsWidget框架");
    window.resize(800, 600);

    // 创建内容部件
    // QWidget *content = new QWidget();

    // window.setContentWidget(content);
    window.show();

    return a.exec();
}
