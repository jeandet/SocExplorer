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

    void can_Update_Plugin_Lookup_Path()
    {
        auto loader = std::make_unique<PluginManager<fake_logger>>(QStringList{});
        auto plugin = loader->makeInstance("SimplePlugin");
        QVERIFY(plugin==Q_NULLPTR);
        loader->addPluginLookupPath(PLUGIN_PATH, true);
        plugin = loader->makeInstance("SimplePlugin");
        QVERIFY(plugin!=Q_NULLPTR);
        plugin->setInstanceName("SimplePlugin0");
        QCOMPARE(QStringLiteral("SimplePlugin0"), plugin->instanceName());
    }

    void can_Read_Plugin_Metadata()
    {
        QCOMPARE(QStringLiteral("SimplePlugin"), plugin_manager->pluginName("SimplePlugin"));
        QCOMPARE(QStringLiteral("Alexis Jeandet"), plugin_manager->pluginAuthor("SimplePlugin"));
        QCOMPARE(QStringLiteral("1.0"), plugin_manager->pluginVersion("SimplePlugin"));
    }

    void can_Instanciate_Simple_Plugin()
    {
        auto plugin = plugin_manager->makeInstance("SimplePlugin");
        QVERIFY(plugin!=Q_NULLPTR);
        plugin->setInstanceName("SimplePlugin0");
        QCOMPARE(QStringLiteral("SimplePlugin0"), plugin->instanceName());
    }

    void can_t_Instanciate_Wrong_Plugin()
    {
        auto plugin = plugin_manager->makeInstance("WrongPlugin");
        QVERIFY(plugin==Q_NULLPTR);
    }
};



QTEST_MAIN(APluginLoader)
#include "main.moc"
