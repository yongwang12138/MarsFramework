#ifndef MARSRADIOBUTTON_H
#define MARSRADIOBUTTON_H

#include <QRadioButton>
#include "MarsDef.h"

class MarsRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit MarsRadioButton(QWidget* parent = nullptr);
    explicit MarsRadioButton(const QString& text, QWidget* parent = nullptr);
    ~MarsRadioButton() = default;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private slots:
    void onThemeChanged(MarsThemeType::ThemeMode themeMode);

private:
    MarsThemeType::ThemeMode _themeMode;
};

#endif // MARSRADIOBUTTON_H
