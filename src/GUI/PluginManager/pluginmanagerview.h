#ifndef PLUGINMANGERVIEW_H
#define PLUGINMANGERVIEW_H

#include <QWidget>
#include <isocexplorerplugin.h>
#include <QVector>

namespace Ui {
class PluginMangerView;
}

class PluginManagerView : public QWidget
{
    Q_OBJECT

public:
    explicit PluginManagerView(QWidget *parent = 0);
    ~PluginManagerView();

signals:
    void updatepluginInfo(const QString libname);
    void loadSysDrviver(const QString name);
    void loadSysDriverToParent(const QString name, const QString instanceName);
    void geteplugintree(void);
    void treeChanged(const std::vector<std::shared_ptr<ISocexplorerPlugin>>& drivers);
    void changeSysDriverInstName(const QString newinstanceName,const QString previnstanceName);
    void closeSysDriver(const QString instanceName);
    void pluginselected(const QString& instanceName);

private:
    Ui::PluginMangerView *ui;
};

#endif // PLUGINMANGERVIEW_H
