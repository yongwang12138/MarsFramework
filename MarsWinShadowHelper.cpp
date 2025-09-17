#include "MarsWinShadowHelper.h"

#include <dwmapi.h>

MarsWinShadowHelper::MarsWinShadowHelper(QObject* parent)
    : QObject{parent}
{}

MarsWinShadowHelper& MarsWinShadowHelper::instance()
{
    static MarsWinShadowHelper instance;
    return instance;
}

bool MarsWinShadowHelper::getIsCompositionEnabled() const
{
    // 检查系统是否启用了桌面窗口管理器(DWM)
    BOOL isCompositionEnabled = false;
    DwmIsCompositionEnabled(&isCompositionEnabled);
    return isCompositionEnabled;
}

quint32 MarsWinShadowHelper::getDpiForWindow(const HWND hwnd)
{
    return GetDpiForWindow(hwnd);
}

int MarsWinShadowHelper::getSystemMetricsForDpi(const HWND hwnd, const int index)
{
    const quint32 dpi = getDpiForWindow(hwnd);
    return GetSystemMetricsForDpi(index, dpi);
}

quint32 MarsWinShadowHelper::getResizeBorderThickness(const HWND hwnd)
{
    return getSystemMetricsForDpi(hwnd, SM_CXSIZEFRAME) + getSystemMetricsForDpi(hwnd, SM_CXPADDEDBORDER);
}
