#include <QtTest>
#include <QObject>
#include <QString>
#include <memtester.h>
#include <memory>
#include <stdint.h>

constexpr std::uint32_t log2(std::size_t n)
{
    return (n > 1) ? 1 + log2(n >> 1) : 0;
}

template<typename T=unsigned int>
class FakeMemory
{
    unsigned int capacity;
    std::vector<T> memory;
public:
    FakeMemory(unsigned int capacity)
        :capacity(capacity)
    {
        memory.resize(capacity);
    }

    unsigned int Read(unsigned int* Value, int count, uint64_t address)
    {
        for(int i=0; i<count; i++)
        {
            Value[i] = static_cast<T>(memory[((address>>(log2(sizeof(T)))) + i)%capacity]);
        }
        return static_cast<unsigned int>(count);
    }

    unsigned int Write(unsigned int* Value, int count, uint64_t address)
    {
        for(int i=0; i<count; i++)
        {
            auto shift = log2(sizeof(T));
            memory[((address>>(shift)) + i)%capacity] = static_cast<T>(Value[i]);
        }
        return static_cast<unsigned int>(count);
    }
};

class AMemTester: public QObject
{
    Q_OBJECT
public:
    AMemTester(QObject* parent=Q_NULLPTR)
        :QObject(parent)
    {
    }
private slots:
    void canMeasureAWorking32BitsMemory_data()
    {
        QTest::addColumn<unsigned int>("capacity");

        QTest::newRow("1 Word") << 1u;
        QTest::newRow("16 Word") << 16u;
        QTest::newRow("256 Words") << 256u;
        QTest::newRow("256k Words") << 256u*1024u;
        QTest::newRow("256M Words") << 256u*1024u*1024u;
    }

    void canMeasureAWorking32BitsMemory()
    {
        QFETCH(unsigned int, capacity);
        auto mem = FakeMemory{capacity};
        auto Read = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Read(Value, count, address);};
        auto Write = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Write(Value, count, address);};

        auto result = MemTester::measureMemSize<32>(Write, Read, 0);
        QCOMPARE(result/4, capacity);

    }

    void canMeasureAWorking64BitsMemory_data()
    {
        QTest::addColumn<unsigned int>("capacity");

        QTest::newRow("1 Word") << 1u;
        QTest::newRow("16 Word") << 16u;
        QTest::newRow("256 Words") << 256u;
        QTest::newRow("256k Words") << 256u*1024u;
        QTest::newRow("256M Words") << 256u*1024u*1024u;
    }

    void canMeasureAWorking64BitsMemory()
    {
        QFETCH(unsigned int, capacity);
        auto mem = FakeMemory<uint64_t>{capacity};
        auto Read = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Read(Value, count, address);};
        auto Write = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Write(Value, count, address);};

        auto result = MemTester::measureMemSize<64>(Write, Read, 0);
        QCOMPARE(result/8, capacity);

    }

};



QTEST_MAIN(AMemTester)
#include "main.moc"
