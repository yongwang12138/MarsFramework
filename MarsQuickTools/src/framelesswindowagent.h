#pragma once

#include <QObject>
#include <QPointer>
#include <QWindow>
#include <qqmlintegration.h>

#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
#endif

class FramelessWindowAgent : public QObject
#ifdef Q_OS_WIN
    , public QAbstractNativeEventFilter
#endif
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int titleBarHeight READ titleBarHeight WRITE setTitleBarHeight NOTIFY titleBarHeightChanged)
    Q_PROPERTY(int resizeBorderThickness READ resizeBorderThickness WRITE setResizeBorderThickness NOTIFY resizeBorderThicknessChanged)
    Q_PROPERTY(int dragRegionRight READ dragRegionRight WRITE setDragRegionRight NOTIFY dragRegionRightChanged)

public:
    explicit FramelessWindowAgent(QObject* parent = nullptr);
    ~FramelessWindowAgent() override;

    int titleBarHeight() const;
    void setTitleBarHeight(int value);

    int resizeBorderThickness() const;
    void setResizeBorderThickness(int value);

    int dragRegionRight() const;
    void setDragRegionRight(int value);

    Q_INVOKABLE void attach(QObject* windowObject);
    Q_INVOKABLE void minimize();
    Q_INVOKABLE void maximize();
    Q_INVOKABLE void restore();
    Q_INVOKABLE void toggleMaximize();
    Q_INVOKABLE void closeWindow();

#ifdef Q_OS_WIN
    bool nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) override;
#endif

signals:
    void titleBarHeightChanged();
    void resizeBorderThicknessChanged();
    void dragRegionRightChanged();

private:
    QWindow* window() const;
#ifdef Q_OS_WIN
    qintptr nativeHwnd() const;
    void ensureNativeWindowStyle();
#endif

private:
    QPointer<QWindow> window_{};
    int title_bar_height_{36};
    int resize_border_thickness_{8};
    int drag_region_right_{0};
#ifdef Q_OS_WIN
    qintptr native_hwnd_{0};
    bool native_style_initialized_{false};
#endif
};
