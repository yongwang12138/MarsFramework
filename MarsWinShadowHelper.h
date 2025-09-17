#ifndef MARSWINSHADOWHELPER_H
#define MARSWINSHADOWHELPER_H

#include <QObject>

// #include <windows.h>
// #include <windowsx.h>
#include <dwmapi.h>

#define mWinHelper MarsWinShadowHelper::instance()
class MarsWinShadowHelper : public QObject
{
    Q_OBJECT
private:
    explicit MarsWinShadowHelper(QObject* parent = nullptr);
    MarsWinShadowHelper(const MarsWinShadowHelper&) = delete;
    MarsWinShadowHelper& operator=(const MarsWinShadowHelper&) = delete;
    ~MarsWinShadowHelper() = default;

public:
    static MarsWinShadowHelper& instance();

    bool getIsCompositionEnabled() const;

    quint32 getDpiForWindow(const HWND hwnd);
    int getSystemMetricsForDpi(const HWND hwnd, const int index);
    quint32 getResizeBorderThickness(const HWND hwnd);
};

#endif // MARSWINSHADOWHELPER_H
