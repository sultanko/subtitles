#include "myatomicinteger.h"
#include <QTest>

MyAtomicInteger::MyAtomicInteger(int value, QObject *parent) :
    QObject(parent), value(new QAtomicInt(value))
{
}

MyAtomicInteger::MyAtomicInteger(const MyAtomicInteger &other)
    : QObject(other.parent()), value(new QAtomicInt(other.value->load()))
{
}

MyAtomicInteger::MyAtomicInteger(MyAtomicInteger &&other)
{
    value = other.value;
    other.value = nullptr;
}

MyAtomicInteger::~MyAtomicInteger()
{
    if (value != nullptr)
    {
        delete value;
    }
}

MyAtomicInteger& MyAtomicInteger::operator+=(const MyAtomicInteger& other)
{
    value->fetchAndAddAcquire(other.value->load());
    return *this;
}

MyAtomicInteger operator+(MyAtomicInteger v1, const MyAtomicInteger& v2)
{
    return v1 += v2;
}

int MyAtomicInteger::get() const
{
    return value->load();
}

void MyAtomicInteger::set(int newValue)
{
    this->value->fetchAndStoreAcquire(newValue);
}

MyAtomicInteger& MyAtomicInteger::operator++()
{
    this->value->fetchAndAddAcquire(1);
    return *this;
}

MyAtomicInteger MyAtomicInteger::operator++(int)
{
    MyAtomicInteger old(*this);
    this->value->fetchAndAddAcquire(1);
    return old;
}

MyAtomicInteger& MyAtomicInteger::operator=(const MyAtomicInteger& other)
{
    if (this == &other)
    {
        return *this;
    }
    if (this->value != nullptr)
    {
        delete this->value;
    }
    this->value = new QAtomicInt(other.value->load());
    return *this;
}
