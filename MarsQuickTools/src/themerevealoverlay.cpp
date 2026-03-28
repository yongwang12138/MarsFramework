#include "themerevealoverlay.h"

#include <QPainter>
#include <QPainterPath>
#include <QQuickItem>
#include <QQuickItemGrabResult>
#include <QTimer>
#include <QtMath>

ThemeRevealOverlay::ThemeRevealOverlay(QQuickItem* parent)
    : QQuickPaintedItem(parent)
{
    setVisible(false);
    setAntialiasing(true);
}

ThemeRevealOverlay::~ThemeRevealOverlay()
{
    if (radius_animation_) {
        radius_animation_->stop();
    }
}

qreal ThemeRevealOverlay::radius() const
{
    return radius_;
}

void ThemeRevealOverlay::setRadius(qreal value)
{
    if (qFuzzyCompare(radius_, value)) {
        return;
    }

    radius_ = value;
    emit radiusChanged();
    update();
}

bool ThemeRevealOverlay::running() const
{
    return running_;
}

void ThemeRevealOverlay::prepare(QObject* windowObject, qreal centerX, qreal centerY)
{
    if (running_) {
        return;
    }

    auto* quickWindow = qobject_cast<QQuickWindow*>(windowObject);
    if (!quickWindow || !quickWindow->contentItem()) {
        return;
    }

    // 异步抓取当前窗口内容，避免同步抓图导致点击后的明显卡顿。
    const auto grabResult = quickWindow->contentItem()->grabToImage();
    if (!grabResult) {
        return;
    }

    connect(grabResult.data(), &QQuickItemGrabResult::ready, this, [this, grabResult, quickWindow, centerX, centerY]() {
        const QImage snapshot = grabResult->image();
        if (snapshot.isNull()) {
            return;
        }

        window_ = quickWindow;
        old_window_background_ = snapshot;
        center_ = QPointF(centerX, centerY);
        setRadius(0.0);
        setVisible(true);
        update();

        // 旧画面覆盖层准备完成，通知外层执行“切主题 + 揭幕动画”。
        QTimer::singleShot(0, this, [this]() {
            emit prepared();
        });
    });
}

void ThemeRevealOverlay::reveal(int durationMs)
{
    if (old_window_background_.isNull() || running_) {
        return;
    }

    setRunning(true);

    if (radius_animation_) {
        radius_animation_->stop();
        radius_animation_->deleteLater();
        radius_animation_ = nullptr;
    }

    // 半径动画：从 0 扩散到足够覆盖窗口四角。
    radius_animation_ = new QPropertyAnimation(this, "radius");
    radius_animation_->setDuration(qMax(1, durationMs));
    radius_animation_->setEasingCurve(QEasingCurve::InOutSine);
    radius_animation_->setStartValue(0.0);
    radius_animation_->setEndValue(computeEndRadius());

    connect(radius_animation_, &QPropertyAnimation::finished, this, [this]() {
        setVisible(false);
        old_window_background_ = QImage{};
        setRadius(0.0);
        setRunning(false);
        emit animationFinished();

        if (radius_animation_) {
            radius_animation_->deleteLater();
            radius_animation_ = nullptr;
        }
    });

    radius_animation_->start();
}

void ThemeRevealOverlay::paint(QPainter* painter)
{
    if (old_window_background_.isNull()) {
        return;
    }

    painter->save();
    painter->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    painter->setPen(Qt::NoPen);

    QImage animationImage(old_window_background_.size(), QImage::Format_ARGB32_Premultiplied);
    animationImage.fill(Qt::transparent);

    QPainter animationImagePainter(&animationImage);
    animationImagePainter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform, true);
    animationImagePainter.drawImage(old_window_background_.rect(), old_window_background_);

    // SourceOut：圆形区域被挖空，底下新主题画面逐步显露。
    animationImagePainter.setCompositionMode(QPainter::CompositionMode_SourceOut);

    const qreal dpr = old_window_background_.devicePixelRatio() > 0.0 ? old_window_background_.devicePixelRatio() : 1.0;

    QPainterPath clipPath;
    clipPath.addEllipse(QPointF(center_.x() * dpr, center_.y() * dpr), radius_ * dpr, radius_ * dpr);
    animationImagePainter.setClipPath(clipPath);
    animationImagePainter.drawImage(animationImage.rect(), animationImage);
    animationImagePainter.end();

    painter->drawImage(boundingRect(), animationImage);
    painter->restore();
}

qreal ThemeRevealOverlay::computeEndRadius() const
{
    const qreal dx = qMax(center_.x(), width() - center_.x());
    const qreal dy = qMax(center_.y(), height() - center_.y());
    return qSqrt(dx * dx + dy * dy);
}

void ThemeRevealOverlay::setRunning(bool value)
{
    if (running_ == value) {
        return;
    }

    running_ = value;
    emit runningChanged();
}
