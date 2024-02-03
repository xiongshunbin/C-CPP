// queuetp.h -- queue template with a nested class
#ifndef __QUEUETP_H__
#define __QUEUETP_H__

template<typename Item>
class QueueTP
{
private:
    enum {Q_SIZE = 10};
    // Node is a nested class definition
    class Node
    {
    public:
        Item item;
        Node* next;
        Node(const Item & i) : item(i), next(0) { }
    }
    Node* front;
    Node* rear;
    int items;
    const int qsize;
    QueueTP(const QueueTP & q) : qsize(0) { }
    QueueTP& operator=(const QueueTP & q) { return *this; }
public:
    QueueTP(int qs = Q_SIZE);
    ~QueueTP();
    bool isempty() const
    {
        return items == 0;
    }
    bool isfull() const
    {
        return items == qsize;
    }
    int queuecount() const
    {
        return items;
    }
    bool enqueue(const Item& item);         // add item to end;

};

#endif