#include <QtTest>
#include <QObject>
#include <QString>
#include <Logger.h>
#include <memory>
#include <stdint.h>
#include <sstream>


class ALogger: public QObject
{
    Q_OBJECT
    std::unique_ptr<Logger> logger;
    std::stringstream* sstr;
public:
    ALogger(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
        sstr = new std::stringstream();
        logger = std::make_unique<Logger>(sstr);
    }
private slots:

    void doesntlogsLowerLogLevelMessages()
    {
        this->sstr->clear();
        this->sstr->flush();
        this->logger->setLogLevel(10);
        this->logger->message("Me","This message should be discarded", 9);
        QVERIFY(this->sstr->str().size()==0);

    }

    void logsHigherLogLevelMessages()
    {
        this->sstr->clear();
        this->sstr->flush();
        this->logger->setLogLevel(1);
        this->logger->message("Me","This message should be discarded", 2);
        QVERIFY(this->sstr->str().size()!=0);
    }


    void logsSameLogLevelMessages()
    {
        this->sstr->clear();
        this->sstr->flush();
        this->logger->setLogLevel(1);
        this->logger->message("Me","This message should be discarded", 1);
        QVERIFY(this->sstr->str().size()!=0);
    }

};



QTEST_MAIN(ALogger)
#include "main.moc"
