#ifndef MARSSCROLLPAGEAREA_H
#define MARSSCROLLPAGEAREA_H

#include <QWidget>
#include "MarsDef.h"

class MarsScrollPageArea : public QWidget
{
    Q_OBJECT

public:
    explicit MarsScrollPageArea(QWidget* parent = nullptr);
    ~MarsScrollPageArea() = default;

signals:
    void borderRadiusChanged(int radius);

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    int _borderRadius;
    MarsThemeType::ThemeMode _themeMode;
};

#endif // MARSSCROLLPAGEAREA_H
