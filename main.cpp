#include <QApplication>
#include "MarsWindow.h"
#include "setting_window.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MarsWindow window;
    Setting_Window* setting_winodw = new Setting_Window();

    // 创建页面2 - 浅粉色背景带文本和额外元素
    QWidget* page2 = new QWidget();
    // 设置背景颜色
    QPalette pal2 = page2->palette();
    pal2.setColor(QPalette::Window, QColor(245, 220, 230)); // 浅粉色
    page2->setPalette(pal2);
    page2->setAutoFillBackground(true);

    // 添加页面到主窗口
    window.addCentralWidget(setting_winodw);
    window.addCentralWidget(page2);
    window.show();
    return a.exec();
}
