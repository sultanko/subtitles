#ifndef MYATOMICINTEGER_H
#define MYATOMICINTEGER_H

#include <QObject>
#include <QAtomicInt>

// modern and useful class for atomic operations
// with integer
class MyAtomicInteger : public QObject
{
    Q_OBJECT
public:
    // default constructor
    explicit MyAtomicInteger(int value = 0, QObject *parent = 0);
    // copy constructor
    MyAtomicInteger(const MyAtomicInteger& other);
    // move constructor
    MyAtomicInteger(MyAtomicInteger&& other);

    // virtual destructor for safe delete data
    virtual ~MyAtomicInteger();

    int get() const;
    void set(int newValue);

    MyAtomicInteger& operator=(const MyAtomicInteger& other);

    MyAtomicInteger& operator+=(const MyAtomicInteger& other);
    // pre-increment
    MyAtomicInteger& operator++();
    // post-increment
    MyAtomicInteger operator++(int);

    friend MyAtomicInteger operator+(MyAtomicInteger v1, const MyAtomicInteger& v2);

private:
    QAtomicInt* value;
};

#endif // MYATOMICINTEGER_H
