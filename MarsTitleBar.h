#ifndef MARSTITLEBAR_H
#define MARSTITLEBAR_H

#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>

// 命中测试结果枚举
enum class HitTestResult {
    None,           // 未命中
    TitleBar,       // 命中标题栏
    MinimizeButton, // 命中最小化按钮
    MaximizeButton, // 命中最大化按钮
    CloseButton     // 命中关闭按钮
};

class MarsTitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit MarsTitleBar(QWidget* parent = nullptr);
    ~MarsTitleBar() = default;

    void setTitle(const QString& title);
    void setIcon(const QIcon& icon);

    // 按钮控制
    void setMinimizeButtonVisible(bool visible);
    void setMaximizeButtonVisible(bool visible);
    void setCloseButtonVisible(bool visible);

    // 获取按钮指针
    QToolButton* minimizeButton() const;
    QToolButton* maximizeButton() const;
    QToolButton* closeButton() const;

    // 更新(最大化/恢复)图标
    void updateMaximizeIcon();

    // 命中测试方法
    HitTestResult hitTest(const QPoint& globalPos) const;

#ifdef Q_OS_WIN
    // Windows 原生事件处理
    bool takeOverNativeEvent(MSG* msg, qintptr* result);
#endif

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

private slots:
    void onMinimizeClicked();
    void onMaximizeClicked();
    void onCloseClicked();

private:
    void setupUi();
    bool isPointInButton(const QPoint& point, QToolButton* button) const;

signals:
    void minimizeClicked();
    void maximizeClicked();
    void closeClicked();

private:
    QHBoxLayout* _layout {nullptr};
    QLabel* _iconLabel {nullptr};
    QLabel* _titleLabel {nullptr};
    QToolButton* _minimizeButton {nullptr};
    QToolButton* _maximizeButton {nullptr};
    QToolButton* _closeButton {nullptr};
};

#endif // MARSTITLEBAR_H