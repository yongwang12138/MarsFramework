#ifndef MARSWINDOW_H
#define MARSWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "MarsTitleBar.h"
#include "MarsDef.h"
#include "MarsThemeAnimationWidget.h"

class QVBoxLayout;
class MarsWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MarsWindow(QWidget* parent = nullptr);
    ~MarsWindow() = default;
    void moveToCenter();
    void setNavButtons(const QList<MarsNavButton>& navButtons);
    void addCentralWidget(QWidget* centralWidget);

protected:
    virtual void paintEvent(QPaintEvent* event) override;
    virtual bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private slots:
    void onThemeReadyChange();
    void onThemeModeChanged();

private:
    void initNavButtons();
    qreal _distance(QPoint point1, QPoint point2);

private:
    MarsApplicationType::WindowDisplayMode _windowDisplayMode {MarsApplicationType::Normal};
    MarsThemeType::ThemeMode _themeMode;
    QVBoxLayout* _layout {nullptr};
    MarsTitleBar* _titleBar {nullptr};
    MarsThemeAnimationWidget* _animationWidget {nullptr};
    QStackedWidget*  _centerStackedWidget{nullptr};
};

#endif // MARSWINDOW_H
