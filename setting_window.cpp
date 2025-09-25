#include "setting_window.h"

#include <QButtonGroup>
#include <QComboBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include "MarsApplication.h"
#include "MarsRadioButton.h"
#include "MarsText.h"
#include "MarsScrollPageArea.h"
#include "MarsTheme.h"

Setting_Window::Setting_Window(QWidget* parent)
    : QWidget{parent}
{
    MarsText* themeText = new MarsText("主题设置", this);
    themeText->setWordWrap(false);

    _themeComboBox = new QComboBox(this);
    _themeComboBox->addItem("日间模式");
    _themeComboBox->addItem("夜间模式");

    MarsScrollPageArea* themeSwitchArea = new MarsScrollPageArea(this);
    QHBoxLayout* themeSwitchLayout = new QHBoxLayout(themeSwitchArea);
    MarsText* themeSwitchText = new MarsText("主题切换", this);
    themeSwitchText->setWordWrap(false);
    themeSwitchLayout->addWidget(themeSwitchText);
    themeSwitchLayout->addStretch();
    themeSwitchLayout->addWidget(_themeComboBox);
    connect(_themeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        if (index == 0)
        {
            mTheme.setThemeMode(MarsThemeType::Light);
        }
        else
        {
            mTheme.setThemeMode(MarsThemeType::Dark);
        }
    });
    connect(&mTheme, &MarsTheme::themeModeChanged, this, [=](MarsThemeType::ThemeMode themeMode) {
        _themeComboBox->blockSignals(true);
        if (themeMode == MarsThemeType::Light)
        {
            _themeComboBox->setCurrentIndex(0);
        }
        else
        {
            _themeComboBox->setCurrentIndex(1);
        }
        _themeComboBox->blockSignals(false);
    });

    MarsText* helperText = new MarsText("应用程序设置", this);
    helperText->setWordWrap(false);

    _normalButton = new MarsRadioButton("Normal", this);
    _elaMicaButton = new MarsRadioButton("ElaMica", this);
#ifdef Q_OS_WIN
    _micaButton = new MarsRadioButton("Mica", this);
    _micaAltButton = new MarsRadioButton("Mica-Alt", this);
    _acrylicButton = new MarsRadioButton("Acrylic", this);
    _dwmBlurnormalButton = new MarsRadioButton("Dwm-Blur", this);
#endif
    _normalButton->setChecked(true);
    QButtonGroup* displayButtonGroup = new QButtonGroup(this);
    displayButtonGroup->addButton(_normalButton, 0);
    displayButtonGroup->addButton(_elaMicaButton, 1);
#ifdef Q_OS_WIN
    displayButtonGroup->addButton(_micaButton, 2);
    displayButtonGroup->addButton(_micaAltButton, 3);
    displayButtonGroup->addButton(_acrylicButton, 4);
    displayButtonGroup->addButton(_dwmBlurnormalButton, 5);
#endif
    connect(displayButtonGroup, QOverload<QAbstractButton*, bool>::of(&QButtonGroup::buttonToggled), this, [=](QAbstractButton* button, bool isToggled) {
        if (isToggled)
        {
            mApp.setWindowDisplayMode((MarsApplicationType::WindowDisplayMode)displayButtonGroup->id(button));
        }
    });
    connect(&mApp, &MarsApplication::pWindowDisplayModeChanged, this, [=]() {
        auto button = displayButtonGroup->button(mApp.getWindowDisplayMode());
        QRadioButton* elaRadioButton = dynamic_cast<QRadioButton*>(button);
        if (elaRadioButton)
        {
            elaRadioButton->setChecked(true);
        }
    });
    MarsScrollPageArea* micaSwitchArea = new MarsScrollPageArea(this);
    QHBoxLayout* micaSwitchLayout = new QHBoxLayout(micaSwitchArea);
    MarsText* micaSwitchText = new MarsText("窗口效果", this);
    micaSwitchText->setWordWrap(false);
    micaSwitchLayout->addWidget(micaSwitchText);
    micaSwitchLayout->addStretch();
    micaSwitchLayout->addWidget(_normalButton);
    micaSwitchLayout->addWidget(_elaMicaButton);
#ifdef Q_OS_WIN
    micaSwitchLayout->addWidget(_micaButton);
    micaSwitchLayout->addWidget(_micaAltButton);
    micaSwitchLayout->addWidget(_acrylicButton);
    micaSwitchLayout->addWidget(_dwmBlurnormalButton);
#endif



    // 设置窗口主布局
    QVBoxLayout* centerLayout = new QVBoxLayout(this);
    centerLayout->setContentsMargins(10, 0, 10, 0);
    centerLayout->addWidget(themeText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(themeSwitchArea);
    centerLayout->addWidget(helperText);
    centerLayout->addSpacing(10);
    centerLayout->addWidget(micaSwitchArea);
    centerLayout->addStretch();
}
