#ifndef MARSTEXT_H
#define MARSTEXT_H

#include "MarsDef.h"
#include <QLabel>

class MarsText : public QLabel
{
    Q_OBJECT
public:
    explicit MarsText(QWidget* parent = nullptr);

private slots:
    void onThemeChanged(MarsThemeType::ThemeMode themeMode);

private:
    MarsThemeType::ThemeMode _themeMode;
};

#endif // MARSTEXT_H
