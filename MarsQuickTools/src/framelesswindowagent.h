#pragma once

#include <QObject>
#include <QPointer>
#include <QWindow>
#include <qqmlintegration.h>

#ifdef Q_OS_WIN
#include <QAbstractNativeEventFilter>
#endif

// FramelessWindowAgent：
// 不是窗口本体，而是“无边框窗口行为代理”。
// 负责把 QML Window 和系统窗口行为连接起来（拖动、缩放、最小化/最大化/关闭、Windows 原生命中测试等）
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

    // 标题栏可拖拽高度（逻辑像素）
    int titleBarHeight() const;
    void setTitleBarHeight(int value);

    // 边框缩放命中厚度（逻辑像素）
    int resizeBorderThickness() const;
    void setResizeBorderThickness(int value);

    // 标题栏拖拽区域右边界（通常到系统按钮左侧）
    int dragRegionRight() const;
    void setDragRegionRight(int value);

    // 绑定目标窗口
    Q_INVOKABLE void attach(QObject* windowObject);

    // 窗口控制接口（供 QML 按钮调用）
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
