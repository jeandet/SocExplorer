#include "pluginmanagerview.h"
#include "ui_pluginmanagerview.h"
#include <SOC/SOC.h>
PluginManagerView::PluginManagerView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PluginMangerView)
{
    ui->setupUi(this);
    auto callback = [this](const QHash<QString,std::shared_ptr<ISocexplorerPlugin>>& tree)
    {
        //this->ui->pluginList->
    };
    //SOC::registerPluginUpdateCallback();
    connect(this, &PluginManagerView::treeChanged, this->ui->loadedPluginsTree, &PluginTreeWidget::treeChanged);
//    connect(this,SIGNAL(treeChanged(QList<socexplorerplugin*>)),this->treeview,SLOT(treeChanged(QList<socexplorerplugin*>)));
}

PluginManagerView::~PluginManagerView()
{
    delete ui;
}
