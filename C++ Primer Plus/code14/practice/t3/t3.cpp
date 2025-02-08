#include "queuetp.h"
#include <iostream>

using namespace std;

int main()
{
    QueueTp<Worker *> wk(3);
    Worker w1;
    w1.Set();
    w1.Show();
    wk.enQueue(&w1);
    cout << "Now queue has " << wk.queueCount() << " items." << endl;

    Worker w2;
    w2.Set();
    w2.Show();
    wk.enQueue(&w2);
    cout << "Now queue has " << wk.queueCount() << " items." << endl;

    Worker w3;
    w3.Set();
    w3.Show();
    wk.enQueue(&w3);
    cout << "Now queue has " << wk.queueCount() << " items." << endl;
    return 0;
}