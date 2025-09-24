#include <QApplication>
#include "MarsWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MarsWindow window;
    // 创建页面1 - 浅蓝色背景带文本
    QWidget* page1 = new QWidget();
    // 设置背景颜色
    QPalette pal1 = page1->palette();
    pal1.setColor(QPalette::Window, QColor(220, 230, 245)); // 浅蓝色
    page1->setPalette(pal1);
    page1->setAutoFillBackground(true);

    // 创建页面2 - 浅粉色背景带文本和额外元素
    QWidget* page2 = new QWidget();
    // 设置背景颜色
    QPalette pal2 = page2->palette();
    pal2.setColor(QPalette::Window, QColor(245, 220, 230)); // 浅粉色
    page2->setPalette(pal2);
    page2->setAutoFillBackground(true);

    // 添加页面到主窗口
    window.addCentralWidget(page1);
    window.addCentralWidget(page2);
    window.show();
    return a.exec();
}
