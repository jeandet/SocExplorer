#include <QtTest>
#include <QObject>
#include <QString>
#include <pluginmanager.h>

struct fake_logger
{
    static void message(const QString& sender, const QString& text, int level)
    {
        std::cout << text.toStdString();
    }
};

class APluginLoader: public QObject
{
    Q_OBJECT
private slots:
    void canLoadAPlugin()
    {
        auto plugin_manager = PluginManager<fake_logger>(QStringList{PLUGIN_PATH});
        QCOMPARE("SimplePlugin", plugin_manager.pluginName("SimplePlugin"));
        QCOMPARE("Alexis Jeandet", plugin_manager.pluginAuthor("SimplePlugin"));
        QCOMPARE("1.0", plugin_manager.pluginVersion("SimplePlugin"));
    }
};



QTEST_MAIN(APluginLoader)
#include "main.moc"
