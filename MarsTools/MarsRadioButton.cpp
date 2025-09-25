#include "MarsRadioButton.h"
#include "MarsTheme.h"

MarsRadioButton::MarsRadioButton(QWidget* parent)
    : QRadioButton{parent}
{
    setFixedHeight(20);
    QFont font = this->font();
    font.setPixelSize(15);
    setFont(font);
    onThemeChanged(mTheme.getThemeMode());
    connect(&mTheme, &MarsTheme::themeModeChanged, this, &MarsRadioButton::onThemeChanged);
}

MarsRadioButton::MarsRadioButton(const QString& text, QWidget* parent)
    :MarsRadioButton(parent)
{
    setText(text);
}

void MarsRadioButton::paintEvent(QPaintEvent *event)
{
    if (palette().color(QPalette::WindowText) != mTheme.getThemeColor(_themeMode, MarsThemeType::BasicText))
    {
        onThemeChanged(_themeMode);
    }
    QRadioButton::paintEvent(event);
}

void MarsRadioButton::onThemeChanged(MarsThemeType::ThemeMode themeMode)
{
    _themeMode = themeMode;
    QPalette palette;
    palette.setColor(QPalette::WindowText,  mTheme.getThemeColor(_themeMode, MarsThemeType::BasicText));
    setPalette(palette);
}
