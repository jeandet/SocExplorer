#include "pluginmangerview.h"
#include "ui_pluginmangerview.h"

PluginManagerView::PluginManagerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginMangerView)
{
    ui->setupUi(this);
    connect(this, &PluginManagerView::treeChanged, this->ui->LoadedPluginsTree, &PluginTreeWidget::treeChanged);
//    connect(this,SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->treeview,SLOT(treeChanged(QList<socexplorerplugin*>)));
}

PluginManagerView::~PluginManagerView()
{
    delete ui;
}
