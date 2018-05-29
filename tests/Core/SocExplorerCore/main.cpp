#include <QtTest>
#include <QObject>
#include <QString>
#include <SocExplorerCore.h>
#include <memory>
#include <stdint.h>


class Core: public QObject
{
    Q_OBJECT
public:
    Core(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
    }
private slots:

};



QTEST_MAIN(Core)
#include "main.moc"
