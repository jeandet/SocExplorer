#include <QtTest>
#include <QObject>
#include <QString>
#include <bytes_utils.h>


class A_BytesUtils: public QObject
{
    Q_OBJECT
public:
    A_BytesUtils(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
    }
private slots:

    void can_Swap_32bits_Word()
    {
        QVERIFY(socexplorerBswap32(0x11223344)==0x44332211);
    }

};



QTEST_MAIN(A_BytesUtils)
#include "main.moc"
