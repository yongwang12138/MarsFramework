#ifndef MARSWIDGET_H
#define MARSWIDGET_H

#include <QVBoxLayout>
#include "MarsTitleBar.h"

class MarsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MarsWidget(QWidget* parent = nullptr);
    ~MarsWidget() = default;

    MarsTitleBar* titleBar() const;

    void setContentWidget(QWidget* widget);
    QWidget* contentWidget() const;

protected:
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent* event) override;

private:
    void setupUi();

private:
    QVBoxLayout* _layout {nullptr};
    MarsTitleBar* _titleBar {nullptr};
    QWidget* _contentWidget {nullptr};
};

#endif // MARSWIDGET_H
