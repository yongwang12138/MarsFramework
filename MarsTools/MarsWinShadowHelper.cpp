#include "MarsWinShadowHelper.h"

#ifdef Q_OS_WIN
#include <QDebug>
#include <QWidget>

Q_SINGLETON_CREATE_CPP(MarsWinShadowHelper)

MarsWinShadowHelper::MarsWinShadowHelper(QObject* parent)
    : QObject(parent)
{
    _pIsWinVersionGreater10 = true;
    HMODULE module = LoadLibraryW(L"ntdll.dll");
    if (module)
    {
        auto pRtlGetVersion = reinterpret_cast<RtlGetVersionFunc>(::GetProcAddress(module, "RtlGetVersion"));
        Q_ASSERT(pRtlGetVersion);
        _windowsVersion.dwOSVersionInfoSize = sizeof(_windowsVersion);
        pRtlGetVersion(&_windowsVersion);
        _pIsWinVersionGreater10 = compareWindowsVersion(Win10_Origin);
        _pIsWinVersionGreater11 = compareWindowsVersion(Win11_Origin);
    }
}

MarsWinShadowHelper::~MarsWinShadowHelper()
{
}

bool MarsWinShadowHelper::initWinAPI()
{
    HMODULE dwmModule = LoadLibraryW(L"dwmapi.dll");
    if (dwmModule)
    {
        if (!_dwmExtendFrameIntoClientArea)
        {
            _dwmExtendFrameIntoClientArea = reinterpret_cast<DwmExtendFrameIntoClientAreaFunc>(GetProcAddress(dwmModule, "DwmExtendFrameIntoClientArea"));
        }
        if (!_dwmSetWindowAttribute)
        {
            _dwmSetWindowAttribute = reinterpret_cast<DwmSetWindowAttributeFunc>(GetProcAddress(dwmModule, "DwmSetWindowAttribute"));
        }
        if (!_dwmIsCompositionEnabled)
        {
            _dwmIsCompositionEnabled = reinterpret_cast<DwmIsCompositionEnabledFunc>(GetProcAddress(dwmModule, "DwmIsCompositionEnabled"));
        }
        if (!_dwmEnableBlurBehindWindow)
        {
            _dwmEnableBlurBehindWindow = reinterpret_cast<DwmEnableBlurBehindWindowFunc>(GetProcAddress(dwmModule, "DwmEnableBlurBehindWindow"));
        }
        if (!(_dwmExtendFrameIntoClientArea && _dwmSetWindowAttribute && _dwmIsCompositionEnabled && _dwmEnableBlurBehindWindow))
        {
            qCritical() << "Dwm Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "dwmapi.dll Load Fail!";
        return false;
    }
    HMODULE user32Module = LoadLibraryW(L"user32.dll");
    if (user32Module)
    {
        if (!_setWindowCompositionAttribute)
        {
            _setWindowCompositionAttribute = reinterpret_cast<SetWindowCompositionAttributeFunc>(GetProcAddress(user32Module, "SetWindowCompositionAttribute"));
        }
        if (!_getDpiForWindow)
        {
            _getDpiForWindow = reinterpret_cast<GetDpiForWindowFunc>(GetProcAddress(user32Module, "GetDpiForWindow"));
        }
        if (!_getSystemMetricsForDpi)
        {
            _getSystemMetricsForDpi = reinterpret_cast<GetSystemMetricsForDpiFunc>(GetProcAddress(user32Module, "GetSystemMetricsForDpi"));
        }
        if (!(_setWindowCompositionAttribute && _getDpiForWindow && _getSystemMetricsForDpi))
        {
            qCritical() << "User32 Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "user32.dll Load Fail!";
        return false;
    }

    HMODULE shCoreModule = LoadLibraryW(L"SHCore.dll");
    if (shCoreModule)
    {
        if (!_getDpiForMonitor)
        {
            _getDpiForMonitor = reinterpret_cast<GetDpiForMonitorFunc>(GetProcAddress(shCoreModule, "GetDpiForMonitor"));
        }
        if (!(_getDpiForMonitor))
        {
            qCritical() << "SHCore Func Init Incomplete!";
            return false;
        }
    }
    else
    {
        qCritical() << "SHCore.dll Load Fail!";
        return false;
    }
    return true;
}

void MarsWinShadowHelper::setWindowShadow(quint64 hwnd)
{
    static const MARGINS shadow = {1, 0, 0, 0};
    _dwmExtendFrameIntoClientArea((HWND)hwnd, &shadow);
}

void MarsWinShadowHelper::setWindowThemeMode(quint64 hwnd, bool isLightMode)
{
    if (!compareWindowsVersion(Win10_1809))
    {
        return;
    }
    BOOL bIsLightMode = !isLightMode;
    _DWMWINDOWATTRIBUTE dwAttritube = compareWindowsVersion(Win10_20H1) ? _DWMWA_USE_IMMERSIVE_DARK_MODE : _DWMWA_USE_IMMERSIVE_DARK_MODE_BEFORE_20H1;
    _dwmSetWindowAttribute((HWND)hwnd, dwAttritube, &bIsLightMode, sizeof(bIsLightMode));
}

bool MarsWinShadowHelper::getIsCompositionEnabled() const
{
    BOOL isCompositionEnabled = false;
    _dwmIsCompositionEnabled(&isCompositionEnabled);
    return isCompositionEnabled;
}

bool MarsWinShadowHelper::getIsFullScreen(const HWND hwnd)
{
    RECT windowRect{};
    ::GetWindowRect(hwnd, &windowRect);
    RECT rcMonitor = getMonitorForWindow(hwnd).rcMonitor;
    return windowRect.top == rcMonitor.top && windowRect.left == rcMonitor.left && windowRect.right == rcMonitor.right && windowRect.bottom == rcMonitor.bottom;
}

MONITORINFOEXW MarsWinShadowHelper::getMonitorForWindow(const HWND hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
    MONITORINFOEXW monitorInfo{};
    monitorInfo.cbSize = sizeof(monitorInfo);
    ::GetMonitorInfoW(monitor, &monitorInfo);
    return monitorInfo;
}

quint32 MarsWinShadowHelper::getResizeBorderThickness(const HWND hwnd)
{
    return getSystemMetricsForDpi(hwnd, SM_CXSIZEFRAME) + getSystemMetricsForDpi(hwnd, 92);
}

quint32 MarsWinShadowHelper::getDpiForWindow(const HWND hwnd)
{
    if (_getDpiForWindow)
    {
        return _getDpiForWindow(hwnd);
    }
    else if (_getDpiForMonitor)
    {
        HMONITOR monitor = ::MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
        UINT dpiX{0};
        UINT dpiY{0};
        _getDpiForMonitor(monitor, MDT_EFFECTIVE_DPI, &dpiX, &dpiY);
        return dpiX;
    }
    else
    {
        HDC hdc = ::GetDC(nullptr);
        const int dpiX = ::GetDeviceCaps(hdc, LOGPIXELSX);
        ::ReleaseDC(nullptr, hdc);
        return quint32(dpiX);
    }
}

int MarsWinShadowHelper::getSystemMetricsForDpi(const HWND hwnd, const int index)
{
    const quint32 dpi = getDpiForWindow(hwnd);
    if (_getSystemMetricsForDpi)
    {
        return _getSystemMetricsForDpi(index, dpi);
    }
    const int result = ::GetSystemMetrics(index);
    if (dpi != 96)
    {
        return result;
    }
    const qreal dpr = qreal(dpi) / qreal(96);
    return qRound(qreal(result) / dpr);
}

bool MarsWinShadowHelper::compareWindowsVersion(const QString& windowsVersion) const
{
    QStringList versionList = windowsVersion.split(".");
    if (versionList.count() != 3)
    {
        return false;
    }
    return (_windowsVersion.dwMajorVersion > versionList[0].toUInt()) || (_windowsVersion.dwMajorVersion == versionList[0].toUInt() && (_windowsVersion.dwMinorVersion > versionList[1].toUInt() || _windowsVersion.dwBuildNumber >= versionList[2].toUInt()));
}

void MarsWinShadowHelper::_externWindowMargins(HWND hwnd)
{
    static const MARGINS margins = {65536, 0, 0, 0};
    _dwmExtendFrameIntoClientArea(hwnd, &margins);
}
#endif
