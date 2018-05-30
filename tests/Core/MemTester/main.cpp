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
    std::size_t capacity;
    std::vector<T> memory;
public:
    FakeMemory(std::size_t capacity)
        :capacity(capacity)
    {
        memory.resize(capacity);
    }

    unsigned int Read(unsigned int* Value, int count, uint64_t address)
    {
        if(capacity)
        {
            for(int i=0; i<count; i++)
            {
                Value[i] = static_cast<T>(memory[((address>>(log2(sizeof(T)))) + i)%capacity]);
            }
        }
        return static_cast<unsigned int>(count);
    }

    unsigned int Write(unsigned int* Value, int count, uint64_t address)
    {
        if(capacity)
        {
            for(int i=0; i<count; i++)
            {
                auto shift = log2(sizeof(T));
                memory[((address>>(shift)) + i)%capacity] = static_cast<T>(Value[i]);
            }
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
    void can_Measure_A_Working_32Bits_Memory_data()
    {
        QTest::addColumn<std::size_t>("capacity");

        QTest::newRow("0 Word") << 0ul;
        QTest::newRow("1 Word") << 1ul;
        QTest::newRow("16 Word") << 16ul;
        QTest::newRow("256 Words") << 256ul;
        QTest::newRow("256k Words") << 256ul*1024ul;
        QTest::newRow("256M Words") << 256ul*1024ul*1024ul;
    }

    void can_Measure_A_Working_32Bits_Memory()
    {
        QFETCH(std::size_t, capacity);
        auto mem = FakeMemory{capacity};
        auto Read = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Read(Value, count, address);};
        auto Write = [&mem](unsigned int* Value, int count, uint64_t address)
        {return mem.Write(Value, count, address);};

        auto result = MemTester::measureMemSize<32>(Write, Read, 0);
        QCOMPARE(result/4, capacity);

    }

    void can_Measure_A_Working_64Bits_Memory_data()
    {
        QTest::addColumn<std::size_t>("capacity");

        QTest::newRow("0 Word") << 0ul;
        QTest::newRow("1 Word") << 1ul;
        QTest::newRow("16 Word") << 16ul;
        QTest::newRow("256 Words") << 256ul;
        QTest::newRow("256k Words") << 256ul*1024ul;
        QTest::newRow("256M Words") << 256ul*1024ul*1024ul;
    }

    void can_Measure_A_Working_64Bits_Memory()
    {
        QFETCH(std::size_t, capacity);
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
