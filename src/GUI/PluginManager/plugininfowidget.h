#ifndef PLUGININFOWIDGET_H
#define PLUGININFOWIDGET_H

#include <QWidget>

namespace Ui {
class PluginInfoWidget;
}

class PluginInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PluginInfoWidget(QWidget *parent = Q_NULLPTR);
    ~PluginInfoWidget();

private:
    Ui::PluginInfoWidget *ui;
};

#endif // PLUGININFOWIDGET_H
