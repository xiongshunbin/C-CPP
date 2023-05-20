// arraytp.h -- Array Template
#ifndef ARRAYTP_H_
#define ARRAYTP_H_

#include <iostream>
#include <cstdlib>

template <class T, int n>
class ArrayTP
{
    private:
        T ar[n];
    public:
        ArrayTP() { };
        T & operator[](int i);
        T operator[](int i) const;
};

template <class T, int n>
T & ArrayTP<T, n>::operator[](int i)
{
    if(i < 0 || i >= n)
    {
        std::cerr << "Error in array limits: " << i
            << " is out of range\n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

template <class T, int n>
T ArrayTP<T, n>::operator[](int i) const
{
    if(i < 0 || i >= n)
    {
        std::cerr << "Error in array limits: " << i
            << " is out of range\n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

#endif