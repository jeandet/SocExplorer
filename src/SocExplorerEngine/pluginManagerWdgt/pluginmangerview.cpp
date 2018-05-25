#include "pluginmangerview.h"
#include "ui_pluginmangerview.h"

PluginMangerView::PluginMangerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginMangerView)
{
    ui->setupUi(this);
    connect(this, &PluginMangerView::treeChanged, this->ui->LoadedPluginsTree, &PluginTreeWidget::treeChanged);
//    connect(this,SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->treeview,SLOT(treeChanged(QList<socexplorerplugin*>)));
}

PluginMangerView::~PluginMangerView()
{
    delete ui;
}
