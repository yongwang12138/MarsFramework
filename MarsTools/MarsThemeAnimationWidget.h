#ifndef MARSTHEMEANIMATIONWIDGET_H
#define MARSTHEMEANIMATIONWIDGET_H

#include <QWidget>
#include "MarsProperty.h"

class MarsThemeAnimationWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY_CREATE(qreal, Radius)
    Q_PROPERTY_CREATE(qreal, EndRadius)
    Q_PROPERTY_CREATE(QPoint, Center)
    Q_PROPERTY_CREATE(QImage, OldWindowBackground)
public:
    explicit MarsThemeAnimationWidget(QWidget* parent = nullptr);
    ~MarsThemeAnimationWidget() = default;
    void startAnimation(int msec);

signals:
    void animationFinished();

protected:
    virtual void paintEvent(QPaintEvent* event) override;
};

#endif // MARSTHEMEANIMATIONWIDGET_H
