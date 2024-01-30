#include "stacktp.h"
#include "workermi.h"

template <>
class Stack<Worker *>
{
private:
    enum {MAX = 10};                        // constant specific to class
    Worker* items[MAX];                     // holds stack items 
    int top;                                // index for top stack item
public:
    Stack();
    bool isempty();
    bool isfull();
    bool push(const Worker* & item);        // add item to stack 
    bool pop(Worker* & item);               // pop top into item
};