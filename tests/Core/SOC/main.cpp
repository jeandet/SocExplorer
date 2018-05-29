#include <QtTest>
#include <QObject>
#include <QString>
#include <SOC/SOC.h>
#include <memory>
#include <stdint.h>


class A_SOC: public QObject
{
    Q_OBJECT
public:
    A_SOC(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
    }
private slots:


};



QTEST_MAIN(A_SOC)
#include "main.moc"
