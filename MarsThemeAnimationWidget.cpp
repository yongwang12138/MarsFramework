#include "MarsThemeAnimationWidget.h"

#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
MarsThemeAnimationWidget::MarsThemeAnimationWidget(QWidget* parent)
    : QWidget{parent}
{
    _pEndRadius = 0.01;
}

void MarsThemeAnimationWidget::startAnimation(int msec)
{
    QPropertyAnimation* themeChangeAnimation = new QPropertyAnimation(this, "pRadius");
    themeChangeAnimation->setDuration(msec);
    themeChangeAnimation->setEasingCurve(QEasingCurve::InOutSine);
    connect(themeChangeAnimation, &QPropertyAnimation::finished, this, [=]() {
        emit animationFinished();
        this->deleteLater();
    });
    connect(themeChangeAnimation, &QPropertyAnimation::valueChanged, this, [=](const QVariant& value) {
        Q_UNUSED(value);
        update();
    });
    themeChangeAnimation->setStartValue(0);
    themeChangeAnimation->setEndValue(_pEndRadius);
    themeChangeAnimation->start(QAbstractAnimation::DeleteWhenStopped);
}

void MarsThemeAnimationWidget::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.save();
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.setPen(Qt::NoPen);

    // 1. 创建一个透明图像作为画布
    QImage animationImage(_pOldWindowBackground.size(), QImage::Format_ARGB32);
    animationImage.fill(Qt::transparent);
    // 2. 在画布上先绘制旧界面背景
    QPainter animationImagePainter(&animationImage);
    animationImagePainter.setRenderHints(QPainter::Antialiasing);
    animationImagePainter.drawImage(_pOldWindowBackground.rect(), _pOldWindowBackground);
    // 3. 设置合成模式为"源外"(SourceOut)
    animationImagePainter.setCompositionMode(QPainter::CompositionMode::CompositionMode_SourceOut);
    qreal devicePixelRatioF = _pOldWindowBackground.devicePixelRatioF();
    // 4. 创建圆形裁剪路径(动态变化的半径)
    QPainterPath clipPath;
    clipPath.moveTo(_pCenter.x() * devicePixelRatioF, _pCenter.y() * devicePixelRatioF);
    clipPath.addEllipse(QPointF(_pCenter.x() * devicePixelRatioF, _pCenter.y() * devicePixelRatioF), _pRadius * devicePixelRatioF, _pRadius * devicePixelRatioF);
    animationImagePainter.setClipPath(clipPath);
    animationImagePainter.drawImage(animationImage.rect(), animationImage);
    animationImagePainter.end();

    painter.drawImage(rect(), animationImage);
    painter.restore();
}
