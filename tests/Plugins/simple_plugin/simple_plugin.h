#include <isocexplorerplugin.h>

class SimplePlugin: public ISocexplorerPlugin
{
    Q_OBJECT
    Q_INTERFACES(ISocexplorerPlugin)
    Q_PLUGIN_METADATA(IID "Socexplorer.plugin.SimplePlugin" FILE "simple_plugin.json")
public:
    SimplePlugin() {}
};
