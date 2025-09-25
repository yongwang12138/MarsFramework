#include "MarsText.h"

#include "MarsTheme.h"
MarsText::MarsText(QWidget* parent)
    : QLabel{parent}
{
    _themeMode = mTheme.getThemeMode();
    onThemeChanged(mTheme.getThemeMode());
    connect(&mTheme, &MarsTheme::themeModeChanged, this, &MarsText::onThemeChanged);
}

MarsText::MarsText(QString text, QWidget* parent)
    : MarsText(parent)
{
    setText(text);
}

void MarsText::onThemeChanged(MarsThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    QPalette palette;
    palette.setColor(QPalette::WindowText, mTheme.getThemeColor(_themeMode, MarsThemeType::BasicText));
    setPalette(palette);
}
