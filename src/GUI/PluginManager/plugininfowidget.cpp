#include "plugininfowidget.h"
#include "ui_plugininfowidget.h"

PluginInfoWidget::PluginInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginInfoWidget)
{
    ui->setupUi(this);
}

PluginInfoWidget::~PluginInfoWidget()
{
    delete ui;
}
