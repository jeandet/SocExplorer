#include <QtTest>
#include <QObject>
#include <QString>
#include <pluginmanager.h>
#include <isocexplorerplugin.h>
#include <memory>

struct fake_logger
{
    static void message(const QString& sender, const QString& text, int level)
    {
        std::cerr << sender.toStdString() << ":" << text.toStdString() << std::endl;
    }
};

class APluginLoader: public QObject
{
    Q_OBJECT
    std::unique_ptr<PluginManager<fake_logger>> plugin_manager;
public:
    APluginLoader(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
        plugin_manager = std::make_unique<PluginManager<fake_logger>>(QStringList{PLUGIN_PATH});
    }
private slots:
    void canReadPluginMetadata()
    {
        QCOMPARE("SimplePlugin", plugin_manager->pluginName("SimplePlugin"));
        QCOMPARE("Alexis Jeandet", plugin_manager->pluginAuthor("SimplePlugin"));
        QCOMPARE("1.0", plugin_manager->pluginVersion("SimplePlugin"));
    }

    void canInstanciateSimplePlugin()
    {
        auto plugin = plugin_manager->makeInstance("SimplePlugin");
        QVERIFY(plugin!=Q_NULLPTR);
        plugin->setInstanceName("SimplePlugin0");
        QCOMPARE("SimplePlugin0", plugin->instanceName());
    }

    void cantInstanciateWrongPlugin()
    {
        auto plugin = plugin_manager->makeInstance("WrongPlugin");
        QVERIFY(plugin==Q_NULLPTR);
    }
};



QTEST_MAIN(APluginLoader)
#include "main.moc"
