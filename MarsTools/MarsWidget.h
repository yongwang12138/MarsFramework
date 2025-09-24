#ifndef MARSWIDGET_H
#define MARSWIDGET_H

#include <QWidget>

#include "MarsTitleBar.h"
#include "MarsDef.h"
#include "MarsThemeAnimationWidget.h"

class QVBoxLayout;
class MarsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarsWidget(QWidget* parent = nullptr);
    ~MarsWidget() = default;
    void moveToCenter();
    void setMainWidget(QWidget* widget);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private slots:
    void onThemeReadyChange();

private:
    qreal _distance(QPoint point1, QPoint point2);

private:
    MarsApplicationType::WindowDisplayMode _windowDisplayMode {MarsApplicationType::Normal};
    MarsThemeType::ThemeMode _themeMode;
    QVBoxLayout* _layout {nullptr};
    MarsTitleBar* _titleBar {nullptr};
    QWidget* _mainWidget {nullptr};
    MarsThemeAnimationWidget* _animationWidget {nullptr};
};

#endif // MARSWIDGET_H
