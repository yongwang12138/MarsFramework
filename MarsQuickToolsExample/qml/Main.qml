import QtQuick
import MarsQuickTools
import "pages" as Pages

MarsApplicationWindow {
    windowTitle: AppController.appName
    themeRevealDurationMs: 300
    page: Pages.HomePage {}
}
