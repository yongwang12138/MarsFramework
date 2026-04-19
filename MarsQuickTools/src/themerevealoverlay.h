#pragma once

#include <QImage>
#include <QPointer>
#include <QPropertyAnimation>
#include <QQuickPaintedItem>
#include <QQuickWindow>
#include <qqmlintegration.h>

// ThemeRevealOverlay：
// 主题切换时的“圆形揭幕”覆盖层
// 工作方式：先抓取旧主题截图覆盖在窗口上，再通过半径动画挖空圆形区域，逐步露出新主题
class ThemeRevealOverlay : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit ThemeRevealOverlay(QQuickItem* parent = nullptr);
    ~ThemeRevealOverlay() override;

    qreal radius() const;
    void setRadius(qreal value);

    bool running() const;

    // 先准备旧画面覆盖图
    Q_INVOKABLE void prepare(QObject* windowObject, qreal centerX, qreal centerY);
    // 再执行揭幕动画
    Q_INVOKABLE void reveal(int durationMs = 1200);

    void paint(QPainter* painter) override;

signals:
    void radiusChanged();
    void runningChanged();
    void prepared();
    void animationFinished();

private:
    qreal computeEndRadius() const;
    void setRunning(bool value);

private:
    QPointer<QQuickWindow> window_{};
    QPropertyAnimation* radius_animation_{nullptr};
    QImage old_window_background_{};
    QPointF center_{};
    qreal radius_{0.0};
    bool running_{false};
};
