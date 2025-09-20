#include "widget.h"
#include "ui_widget.h"

#include "MarsTheme.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pushButton_clicked()
{
    if(mTheme.getThemeMode() == MarsThemeType::Light) {
        mTheme.setThemeMode(MarsThemeType::Dark);
        qDebug() << "mTheme.setThemeMode(MarsThemeType::Dark);";
    } else if(mTheme.getThemeMode() == MarsThemeType::Dark) {
        mTheme.setThemeMode(MarsThemeType::Light);
        qDebug() << "mTheme.setThemeMode(MarsThemeType::Light);";
    }
}

