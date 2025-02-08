#ifndef __QUEUETP_H__
#define __QUEUETP_H__

#include <iostream>
#include <string>

using std::cin;
using std::cout;
using std::endl;
using std::string;

class Worker
{
private:
    string fullname;
    long id;
public:
    Worker() : fullname("no one"), id(0L) { }
    Worker(const string & s, long n) : fullname(s), id(n) { }
    ~Worker() { };
    void Set();
    void Show() const;
};

template <typename Type>
class QueueTp
{
private:
    enum { Q_SIZE = 10 };
    struct Node
    {
        Type item;
        Node* next;
    };
    Node* front;
    Node* rear;
    int items;
    const int qsize;
public:
    QueueTp(int qs = Q_SIZE);
    ~QueueTp();
    bool isEmpty() const;
    bool isFull() const;
    int queueCount() const;
    bool enQueue(const Type& item);
    bool deQueue(Type& item);
};

template <typename Type>
inline QueueTp<Type>::QueueTp(int qs) : qsize(qs)
{
    front = nullptr;
    rear = nullptr;
    items = 0;
}

template <typename Type>
QueueTp<Type>::~QueueTp()
{
    Node* temp;
    while(front != nullptr)
    {
        temp = front;
        front = front->next;
        delete temp;
    }
}

template <typename Type>
bool QueueTp<Type>::isEmpty() const
{
    return items == 0;
}

template <typename Type>
bool QueueTp<Type>::isFull() const
{
    return items == qsize;
}

template <typename Type>
bool QueueTp<Type>::enQueue(const Type &item)
{
    if(isFull())      return false;
    Node* add = new Node;
    add->item = item;
    add->next = nullptr;
    items ++;
    if(front == nullptr)
        front = add;
    else
        rear->next = add;
    rear = add;
    return true;
}

template <typename Type>
bool QueueTp<Type>::deQueue(Type &item)
{
    if(front == nullptr)    return false;
    item = front->item;
    items --;
    Node* temp = front;
    front = front->next;
    delete temp;

    if(items == 0)
        rear = nullptr;
    return true;
}

template <typename Type>
int QueueTp<Type>::queueCount() const
{
    return items;
}

#endif

