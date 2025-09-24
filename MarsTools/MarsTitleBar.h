#ifndef MARSTITLEBAR_H
#define MARSTITLEBAR_H

#include <QWidget>

#include "MarsDef.h"
#include "MarsText.h"
#include <QToolButton>

class QLabel;
class QScreen;
class QHBoxLayout;
class QVBoxLayout;
class QMenu;
class MarsTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit MarsTitleBar(QWidget* parent = nullptr);
    ~MarsTitleBar() = default;

    void setWindowButtonFlag(MarsTitleBarType::ButtonType buttonFlag, bool isEnable = true);
    void setWindowButtonFlags(MarsTitleBarType::ButtonFlags buttonFlags);

    MarsTitleBarType::ButtonFlags getWindowButtonFlags() const;

    void setNavButtons(const QList<MarsNavButton>& navButtons);

    void setCurrentNavButton(int index);

#ifdef Q_OS_WIN
    int takeOverNativeEvent(const QByteArray& eventType, void* message, qintptr* result);
#endif

protected:
    virtual bool eventFilter(QObject* obj, QEvent* event) override;
#ifdef Q_OS_WIN
    virtual void paintEvent(QPaintEvent* event) override;
#endif

private:
    void _showAppBarMenu(QPoint point);
    void _updateCursor(int edges);
    bool _containsCursorToItem(QWidget* item);

private slots:
    void onNavButtonClicked();
    void onMinButtonClicked();
    void onMaxButtonClicked();
    void onCloseButtonClicked();

signals:
    void navButtonClicked(int index);
    void themeButtonClicked();
    void closeButtonClicked();

private:
    MarsThemeType::ThemeMode _themeMode;
    QHBoxLayout* _mainLayout {nullptr};
    QLabel* _iconLabel {nullptr};
    MarsText* _titleLabel {nullptr};
    MarsTitleBarType::ButtonFlags _buttonFlags;
    QToolButton* _themeButton {nullptr};
    QToolButton* _minButton {nullptr};
    QToolButton* _maxButton {nullptr};
    QToolButton* _closeButton {nullptr};
    QScreen* _lastScreen {nullptr};
    qint64 _currentWinID {0};
    quint64 _clickTimer {0};
    int _edges {0};
    int _margins {8};
    bool _isHoverMaxButton {false};

    QHBoxLayout* _navButtonLayout {nullptr};
    QList<QToolButton*> _navButtons;
    int _currentNavIndex {-1};
};

#endif // MARSTITLEBAR_H
