#ifndef SETTING_WINDOW_H
#define SETTING_WINDOW_H

#include <QRadioButton>
#include <QWidget>

class QComboBox;
class MarsRadioButton;
class Setting_Window : public QWidget
{
    Q_OBJECT
public:
    explicit Setting_Window(QWidget* parent = nullptr);

private:
    QComboBox* _themeComboBox {nullptr};
    MarsRadioButton* _normalButton{nullptr};
    MarsRadioButton* _elaMicaButton{nullptr};
#ifdef Q_OS_WIN
    MarsRadioButton* _micaButton{nullptr};
    MarsRadioButton* _micaAltButton{nullptr};
    MarsRadioButton* _acrylicButton{nullptr};
    MarsRadioButton* _dwmBlurnormalButton{nullptr};
#endif
};

#endif // SETTING_WINDOW_H
