#ifndef MARSTHEME_H
#define MARSTHEME_H

#include <QObject>
#include <QColor>
#include "MarsDef.h"
#include "MarsSingleton.h"

class QPainter;
class MarsTheme : public QObject
{
    Q_OBJECT
    Q_SINGLETON_CREATE_H(MarsTheme)

private:
    explicit MarsTheme(QObject* parent = nullptr);
    ~MarsTheme() = default;

public:
    void setThemeMode(MarsThemeType::ThemeMode themeMode);
    MarsThemeType::ThemeMode getThemeMode() const;

    void drawEffectShadow(QPainter* painter, QRect widgetRect, int shadowBorderWidth, int borderRadius);

    void setThemeColor(MarsThemeType::ThemeMode themeMode, MarsThemeType::ThemeColor themeColor, QColor newColor);
    const QColor& getThemeColor(MarsThemeType::ThemeMode themeMode, MarsThemeType::ThemeColor themeColor);

Q_SIGNALS:
    void themeModeChanged(MarsThemeType::ThemeMode themeMode);

private:
    void initThemeColor();

    MarsThemeType::ThemeMode _themeMode {MarsThemeType::Light};
    QColor _lightThemeColorList[42];
    QColor _darkThemeColorList[42];
};

#endif // MARSTHEME_H
